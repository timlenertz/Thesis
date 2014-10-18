#include "mmap_allocator.h"

#include <utility>
#include <stdexcept>
#include <map>
#include <cstdint>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/filesystem.hpp>


namespace pcf {

struct mmap_allocator_base::impl {
	struct allocation {
		boost::iostreams::mapped_file file;
		boost::filesystem::path path;
	};

	std::map<std::uintptr_t, std::unique_ptr<allocation>> allocations;
};


std::unique_ptr<mmap_allocator_base::impl> mmap_allocator_base::impl_ = 
	std::unique_ptr<mmap_allocator_base::impl>();
	

mmap_allocator_base::impl& mmap_allocator_base::get_impl_() {
	if(! impl_) impl_.reset(new impl);
	return *impl_;
}


mmap_allocator_base::mmap_allocator_base(const std::string& name):
	name_(name) { }

std::uintptr_t mmap_allocator_base::key_for_ptr_(const void* ptr) {
	return std::uintptr_t(ptr);
}

void* mmap_allocator_base::allocate_(std::size_t length, std::size_t align, const void* hint) {
	if(boost::iostreams::mapped_file::alignment() % align != 0)
		throw std::runtime_error("Memory mapped file not property aligned.");


	boost::filesystem::path model(name_ + "_%%%%-%%%%-%%%%-%%%%");
	boost::filesystem::path path = boost::filesystem::unique_path(model);

	boost::iostreams::mapped_file_params params;
	params.path = path.c_str();
	params.flags = boost::iostreams::mapped_file::readwrite;
	params.new_file_size = length;
	params.hint = reinterpret_cast<const char*>(hint);
	
	impl::allocation* alloc = new impl::allocation;
	alloc->path = path;
	
	alloc->file.open(params);
	void* ptr = reinterpret_cast<void*>(alloc->file.data());

	auto& impl = get_impl_();
	std::uintptr_t key = key_for_ptr_(ptr);
	impl.allocations[key].reset(alloc);
		
	return ptr;
}

void mmap_allocator_base::deallocate_(void* ptr, std::size_t length) {
	auto& impl = get_impl_();
	std::uintptr_t key = key_for_ptr_(ptr);

	auto it = impl.allocations.find(key);
	if(it == impl.allocations.end()) return;
	if(! it->second) return;
	impl::allocation& alloc = *(it->second);
	
	alloc.file.close();
	boost::filesystem::remove(alloc.path);
	
	impl.allocations.erase(it);
}


}
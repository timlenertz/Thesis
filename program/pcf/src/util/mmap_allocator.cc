#include "mmap_allocator.h"

#include <stdexcept>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/filesystem.hpp>

namespace pcf {

struct mmap_allocator_base::impl {
	boost::filesystem::path path;
	boost::iostreams::mapped_file file;
};


mmap_allocator_base::mmap_allocator_base(const std::string& pathname) :
impl_(new impl) {
	if(! pathname.empty()) impl_->path = boost::filesystem::path(pathname);
	else impl_->path = boost::filesystem::unique_path();
}

mmap_allocator_base::~mmap_allocator_base() { }


void* mmap_allocator_base::allocate_(std::size_t length, std::size_t align, const void* hint) {
	if(impl_->file.alignment() % align != 0) throw std::runtime_error("Memory mapped file not property aligned.");

	boost::iostreams::mapped_file_params params(impl_->path.native());
	params.flags = boost::iostreams::mapped_file::readwrite;
	params.new_file_size = length;
	params.hint = reinterpret_cast<const char*>(hint);
	
	impl_->file.open(params);
	
	return reinterpret_cast<void*>(impl_->file.data());
}

void mmap_allocator_base::deallocate_(void* buf, std::size_t length) {
	impl_->file.close();
}

void mmap_allocator_base::remove_file_() {
	boost::filesystem::remove(impl_->path);
}


}
#ifdef __APPLE__

#include "mmap_allocator.h"

#include <utility>
#include <stdexcept>
#include <map>
#include <cstdint>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <cstdio>

namespace pcf {

struct mmap_allocator_base::impl {
	struct allocation {
		int file;
		std::string path;
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
	// Round length up to system page size
	int sys_page_size = getpagesize();
	length /= sys_page_size;
	length *= (sys_page_size + 1);
	
	if(sys_page_size % align != 0)
		throw std::runtime_error("System page size not a multiple of requested alignment.");


	// Generate temporary file with unique name
	std::unique_ptr<char[]> name_tmp( new char[name_.size() + 8] );
	
	std::memcpy(name_tmp.get(), name_.data(), name_.size());
	std::memcpy(name_tmp.get() + name_.size(), "_XXXXXX", 8);
	
	int fd = mkstemp(name_tmp.get());
	if(fd == -1) throw std::runtime_error("Could not create temporary file.");
		

	// Make sparse file of requested length
	ftruncate(fd, length);
	
	// Map file to virtual memory
	void* ptr = mmap(
		const_cast<void*>(hint),
		length,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		fd,
		0
	);
	if((std::intptr_t)ptr == -1) throw std::runtime_error("Could not create mmap region.");

	// Add to allocations list	
	std::uintptr_t key = key_for_ptr_(ptr);
	auto& impl = get_impl_();
	impl::allocation* alloc = new impl::allocation;
	alloc->path = name_tmp.get();
	alloc->file = fd;
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
	
	munmap((caddr_t)ptr, length);
	unlink(alloc.path.c_str());
	close(alloc.file);
		
	impl.allocations.erase(it);
}

}

#endif
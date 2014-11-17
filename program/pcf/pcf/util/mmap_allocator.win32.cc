#ifdef _WIN32

#include "mmap_allocator.h"

#include <utility>
#include <stdexcept>
#include <map>

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
	return nullptr;
}


void mmap_allocator_base::deallocate_(void* ptr, std::size_t length) {
}

}

#endif
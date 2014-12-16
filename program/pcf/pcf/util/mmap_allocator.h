#ifndef PCF_MMAP_ALLOCATOR_H_
#define PCF_MMAP_ALLOCATOR_H_

#include <string>
#include <cassert>
#include <cstdint>
#include <memory>
#include "memory.h"

namespace pcf {

/**
Non template base class for mmap allocation.
Implementation in .cc files uses operating system specific API.
*/
class mmap_allocator_base {
private:
	struct impl;
	static std::unique_ptr<impl> impl_;
	
	std::string name_;
	
	static impl& get_impl_();
	std::uintptr_t key_for_ptr_(const void*);
		
public:
	explicit mmap_allocator_base(const std::string& name);
	
	mmap_allocator_base(const mmap_allocator_base&) = default;
	
	void* allocate_(std::size_t length, std::size_t align, const void* hint);
	void deallocate_(void* buf, std::size_t length);
};



/*
Allocator which creates memory-mapped files.
Allows for allocating very large memory segments. File paths are prefixed with given name, and
appended with unique code per allocation. Allocations list is shared among all mmap_allocator's
regardless of type and name. 
*/
template<class T>
class mmap_allocator : private mmap_allocator_base {
public:
	using value_type = T;

public:
	mmap_allocator(const std::string& name = std::string("pcf")) :
		mmap_allocator_base(name) { }
	
	mmap_allocator(const mmap_allocator&) = default;
	mmap_allocator& operator=(const mmap_allocator&) = default;

	T* allocate(std::size_t n, const T* hint = 0) {
		return reinterpret_cast<T*>(
			allocate_(n * sizeof(T), ALIGNOF(T), reinterpret_cast<const void*>(hint))
		);
	}
	
	void deallocate(T* buf, std::size_t n) {
		deallocate_(reinterpret_cast<T*>(buf), n * sizeof(T));
	}
};

}

#endif

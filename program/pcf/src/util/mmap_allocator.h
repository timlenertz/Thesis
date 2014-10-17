#ifndef PCF_MMAP_ALLOCATOR_H_
#define PCF_MMAP_ALLOCATOR_H_

#include <string>
#include <cassert>
#include <memory>

namespace pcf {

class mmap_allocator_base {
private:
	struct impl;
	std::unique_ptr<impl> impl_;
	bool allocated_ = false;

public:
	mmap_allocator_base(const std::string& path);
	mmap_allocator_base(const mmap_allocator_base&) = delete;
	mmap_allocator_base(mmap_allocator_base&&);
	~mmap_allocator_base();
	
	mmap_allocator_base& operator=(const mmap_allocator_base&) = delete;
	mmap_allocator_base& operator=(mmap_allocator_base&&);
	
	void* allocate_(std::size_t length, std::size_t align, const void* hint);
	void deallocate_(void* buf, std::size_t length);
	void remove_file_();
};


template<class T>
class mmap_allocator : private mmap_allocator_base {
public:
	using value_type = T;

private:
	const bool temporary_;

public:
	mmap_allocator(const std::string& path = std::string(), bool temporary = true) :
	mmap_allocator_base(path), temporary_(temporary) { }
	
	mmap_allocator(mmap_allocator&&) = default;
	mmap_allocator& operator=(mmap_allocator&) = default;

	T* allocate(std::size_t n, const T* hint = 0) {
		return reinterpret_cast<T*>(
			allocate_(n * sizeof(T), alignof(T), reinterpret_cast<void*>(hint))
		);
	}
	
	void deallocate(T* buf, std::size_t n) {
		deallocate_(reinterpret_cast<const T*>(buf), n * sizeof(T));
		if(temporary_) remove_file_();
	}
};

}

#endif

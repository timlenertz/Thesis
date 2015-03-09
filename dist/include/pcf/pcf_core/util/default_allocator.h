#ifndef PCF_DEFAULT_ALLOCATOR_H_
#define PCF_DEFAULT_ALLOCATOR_H_

#include "mmap_allocator.h"
#include "aligned_allocator.h"

namespace pcf {

template<typename T>
class default_allocator {
private:
	const static std::size_t mmap_threshold_;

	mmap_allocator<T> mmap_allocator_;
	aligned_allocator<T> standard_allocator_;

public:
	T* allocate(std::size_t n, const T* hint = 0);
	void deallocate(T* buf, std::size_t n);
};

}

#include "default_allocator.tcc"

#endif
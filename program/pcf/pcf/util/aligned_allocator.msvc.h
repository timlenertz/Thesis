#ifndef PCF_ALIGNED_ALLOCATOR_MSVC_H_
#define PCF_ALIGNED_ALLOCATOR_MSVC_H_

#ifndef _MSC_VER
#error Only for Visual C++
#endif

#include <malloc.h>

namespace pcf {

template<typename T>
class aligned_allocator {
public:
	T* allocate(std::size_t n, const T* hint = 0) {
		void* ptr = _aligned_malloc(n * sizeof(T), __alignof(T));
		return reinterpret_cast<T*>(ptr);
	}
	
	void deallocate(T* buf, std::size_t n) {
		_aligned_free(buf);
	}	
};

}

#endif
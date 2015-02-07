#ifndef PCF_ALIGNED_ALLOCATOR_H_
#define PCF_ALIGNED_ALLOCATOR_H_

#if defined(_MSC_VER) && defined(_WIN32)

	#include <malloc.h>

	namespace pcf {

	/**
	Aligned memory allocator using Win32 API.
	Visual C++ 2013 does not yet support C++11 memory alignment, and so std::allocator
	would not properly align the memory.
	*/
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


#else

	#include <memory>

	namespace pcf {

	/**
	Use standard allocator as aligned allocator.
	Assumes C++11 aligned memory allocator is supported by compiler.
	*/	
	template<typename T> using aligned_allocator = std::allocator<T>;

	}

#endif

#endif
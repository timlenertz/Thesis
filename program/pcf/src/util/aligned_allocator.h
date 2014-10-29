#ifndef PCF_ALIGNED_ALLOCATOR_H_
#define PCF_ALIGNED_ALLOCATOR_H_

#ifdef _MSC_VER
	#include "aligned_allocator.msvc.h"
#elif
	#include <allocator>
	namespace pcf {
	
	template<typename T> using aligned_allocator = std::allocator<T>;
	
	}
#endif

#endif
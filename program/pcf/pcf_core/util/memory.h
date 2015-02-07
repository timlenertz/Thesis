#ifndef PCF_MEMORY_H_
#define PCF_MEMORY_H_

#include <cassert>
#include <cstddef>

#ifdef _MSC_VER
	#define ALIGNAS(n) __declspec( align(n) )
	#define ALIGNOF(t) __alignof(t)
#else
	#define ALIGNAS(n) alignas(n)
	#define ALIGNOF(t) alignof(t)
#endif


namespace pcf {

const extern std::size_t system_page_size;

template<typename T>
std::size_t round_up_to_fit_system_page_size(std::size_t n) {
	assert(system_page_size % sizeof(T) == 0);
	std::size_t sz = sizeof(T) * n;
	return n + (sz % system_page_size)/sizeof(T);
}

enum class memory_usage_advice {
	normal,
	sequential,
	random
};

void set_memory_usage_advice(void*, std::size_t, memory_usage_advice);

}

#endif
#ifdef _WIN32

#include "memory.h"
#include <windows.h.h>

namespace pcf {

static std::size_t get_system_page_size_() {
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwPageSize;
}

const std::size_t system_page_size = get_system_page_size_();

void set_memory_usage_advice(void* buf, std::size_t len, memory_usage_advice adv) {
	// Not available on Windows
	return;
}

}

#endif

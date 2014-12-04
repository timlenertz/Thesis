#ifdef _WIN32

#include "alignment.h"
#include <windows.h.h>

namespace pcf {

static std::size_t get_system_page_size_() {
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.si.dwPageSize;
}

const std::size_t system_page_size = get_system_page_size_();

}

#endif

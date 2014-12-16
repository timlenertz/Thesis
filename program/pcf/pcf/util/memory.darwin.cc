#ifdef __APPLE__

#include "memory.h"
#include <unistd.h>
#include <sys/mman.h>

namespace pcf {

const std::size_t system_page_size = sysconf(_SC_PAGESIZE);

void set_memory_usage_advice(void* buf, std::size_t len, memory_usage_advice adv) {
	switch(adv) {
	case memory_usage_advice::normal:
		madvise(buf, len, MADV_NORMAL);
		break;
	case memory_usage_advice::sequential:
		madvise(buf, len, MADV_SEQUENTIAL);
		break;
	case memory_usage_advice::random:
		madvise(buf, len, MADV_RANDOM);
		break;
	return;	
	}
}

}

#endif
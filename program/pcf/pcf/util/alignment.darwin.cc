#ifdef __APPLE__

#include "alignment.h"
#include <unistd.h>

namespace pcf {

const std::size_t system_page_size = sysconf(_SC_PAGESIZE);

}

#endif
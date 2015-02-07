#ifndef PCF_UTIL_MISC_H_
#define PCF_UTIL_MISC_H_

#include <string>

namespace pcf {

std::string file_name_extension(const std::string& filename);
std::string replace_file_name_extension(const std::string& filename, const std::string& ext);
std::string remove_file_name_extension(const std::string& filename);

}

#endif

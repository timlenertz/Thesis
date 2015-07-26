#ifndef PCF_UTIL_MISC_H_
#define PCF_UTIL_MISC_H_

#include <string>
#include <vector>

namespace pcf {

bool file_exists(const std::string& filepath);

std::string file_name_extension(const std::string& filename);
std::string replace_file_name_extension(const std::string& filename, const std::string& ext);
std::string remove_file_name_extension(const std::string& filename);

template<typename T> std::string to_string(const T&);
template<typename T> T from_string(const std::string&);

std::vector<std::string> explode(char separator, const std::string&);
std::string implode(char separator, const std::vector<std::string>&);

template<typename T>
std::vector<T> explode_from_string(char separator, const std::string&);

template<typename T>
std::string implode_to_string(char separator, const std::vector<T>&);

template<typename Numeric>
Numeric sq(Numeric n) { return n * n; }

}

#include "misc.tcc"

#endif

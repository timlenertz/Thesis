#include "misc.h"
#include <string>
#include <sstream>
#include <cstdio>


namespace pcf {


bool file_exists(const std::string& filepath) {
	auto handle = std::fopen(filepath.c_str(), "r");
	if(handle) {
		std::fclose(handle);
		return true;
	} else {
		return false;
	}
}

std::string file_name_extension(const std::string& filename) {
	auto dot_pos = filename.find_last_of('.');
	if(dot_pos == std::string::npos) return std::string();
	else return filename.substr(dot_pos + 1);
}


std::string replace_file_name_extension(const std::string& filename, const std::string& ext) {
	auto dot_pos = filename.find_last_of('.');
	
	std::string without_ext;
	if(dot_pos == std::string::npos) without_ext = filename;
	else without_ext = filename.substr(0, dot_pos);
	
	if(ext.empty()) return without_ext;
	else return without_ext + '.' + ext;
}


std::string remove_file_name_extension(const std::string& filename) {
	return replace_file_name_extension(filename, "");
}


std::vector<std::string> explode(char separator, const std::string& str) {
	std::vector<std::string> vec;
	std::string::size_type last_pos = 0;
	std::string::size_type pos = str.find(separator);
	while(pos != std::string::npos) {
		std::string::size_type n = pos - last_pos;
		std::string part = str.substr(last_pos, n);
		vec.push_back(part);
		last_pos = pos + 1;
		pos = str.find(separator, last_pos);
	}
	vec.push_back(str.substr(last_pos));
	return vec;
}


std::string implode(char separator, const std::vector<std::string>& vec) {
	std::ostringstream ostr;
	auto last = vec.end() - 1;
	for(auto it = vec.begin(); it != last; ++it) {
		ostr << *it << separator;
	}
	ostr << vec.back();
	return ostr.str();
}



}
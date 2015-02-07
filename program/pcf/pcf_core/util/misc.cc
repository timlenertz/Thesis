#include "misc.h"
#include <string>


namespace pcf {


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


}
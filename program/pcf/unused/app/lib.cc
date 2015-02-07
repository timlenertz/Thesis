#include "lib.h"
#include "shell.h"
#include <string>
#include <memory>
#include <stdexcept>


namespace pcf {


std::string file_name_extension(const std::string& filename) {
	auto dot_pos = filename.find_last_of('.');
	if(dot_pos == std::string::npos) return std::string();
	else return filename.substr(dot_pos + 1);
}


std::string change_file_name_extension(const std::string& filename, const std::string& ext) {
	auto dot_pos = filename.find_last_of('.');
	
	std::string without_ext;
	if(dot_pos == std::string::npos) without_ext = filename;
	else without_ext = filename.substr(0, dot_pos);
	
	if(ext.empty()) return without_ext;
	else return without_ext + '.' + ext;
}

unorganized_point_cloud_full load_point_cloud(const std::string& filename) {
	std::unique_ptr<point_cloud_importer> imp;
	std::string ext = file_name_extension(filename);	
	if(ext == "ply") imp.reset(new ply_importer(filename));			
	else if(ext == "scan") imp.reset(new pointscan_importer(filename));
	else throw std::runtime_error("Unknown point cloud file extension ." + ext);

	std::cout << "Importing ." << ext << " file..." << std::endl;
	unorganized_point_cloud_full pc(*imp);

	return pc;
}

unorganized_point_cloud_full ask_point_cloud() {
	std::string filename = read_line("Point cloud file");
	if(filename.empty()) throw std::runtime_error("No point cloud file given.");
	else return load_point_cloud(filename);
}


range_point_cloud_full load_range_point_cloud(const std::string& filename) {
	std::unique_ptr<range_point_cloud_importer> imp;
	std::string ext = file_name_extension(filename);	
	if(ext == "scan") imp.reset(new pointscan_importer(filename));
	else throw std::runtime_error("Unknown range point cloud file extension ." + ext);

	std::cout << "Importing ." << ext << " file..." << std::endl;
	range_point_cloud_full pc(*imp);

	return pc;	
}

range_point_cloud_full ask_range_point_cloud() {
	std::string filename = read_line("Range point cloud file", "../../../townhall/Scan_005.scan");
	if(filename.empty()) throw std::runtime_error("No point cloud file given.");
	else return load_range_point_cloud(filename);
}



}
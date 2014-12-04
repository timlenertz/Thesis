#include "lib.h"
#include "../shell.h"
#include "../../pcf/pcf.h"
#include <string>
#include <memory>
#include <stdexcept>


namespace pcf {

unorganized_point_cloud_full ask_point_cloud() {
	std::string in_filename = shell::read_line("Point cloud file");
	if(in_filename.empty()) throw std::runtime_error("No point cloud file given.");
	
	auto dot_pos = in_filename.find_last_of('.');
	if(dot_pos == std::string::npos) throw std::runtime_error("No filename extension.");
		
	std::unique_ptr<point_cloud_importer> imp;
	std::string ext = in_filename.substr(dot_pos + 1);
	
	if(ext == "ply") imp.reset(new ply_importer(in_filename));			
	else if(ext == "scan") imp.reset(new pointscan_importer(in_filename));
	else throw std::runtime_error("Unknown point cloud file extension ." + ext);

	std::cout << "Importing ." << ext << " file..." << std::endl;
	unorganized_point_cloud_full pc(*imp);

	return pc;
}

}
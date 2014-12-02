#include "../program.h"
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "../../pcf/io/pointscan_importer.h"
#include "../../pcf/io/ply_importer.h"
#include "../../pcf/point_cloud/unorganized_point_cloud.h"
#include "../../pcf_viewer/scene/bounding_box.h"

using namespace pcf;

PCF_PROGRAM(view) {
	void main() {
		std::string in_filename = shell::read_line("File");
		if(in_filename.empty()) return;
		
		auto dot_pos = in_filename.find_last_of('.');
		if(dot_pos == std::string::npos) throw std::runtime_error("No filename extension.");
			
		std::unique_ptr<point_cloud_importer> imp;
		std::string ext = in_filename.substr(dot_pos + 1);
		
		if(ext == "ply") imp.reset(new ply_importer(in_filename));			
		else if(ext == "scan") imp.reset(new pointscan_importer(in_filename));
		else throw std::runtime_error("Unknown point cloud file extension ." + ext);

		std::cout << "Importing ." << ext << " file..." << std::endl;
		unorganized_point_cloud_full pc(*imp);
		imp.reset();

		std::cout << "Adding to scene..." << std::endl;
		access_viewer_([&pc](viewer& vw) {
			vw->add_point_cloud(pc);
			vw->add_bounding_box(pc.box());
		});
		
		std::cout << "Done." << std::endl;
	}
};

#include "../program.h"
#include <string>
#include <iostream>
#include <stdexcept>

#include "../../pcf/pcf.h"

using namespace pcf;

PCF_PROGRAM(range_point_cloud_to_image) {
	void main() {
		std::string in_filename = shell::read_line("Pointscan file", "../../../townhall/Scan_005.scan");
		if(in_filename.empty()) return;
		
		pointscan_importer in(in_filename);

		std::cout << "Loading point cloud..." << std::endl;
		range_point_cloud_full pc(in);
		
		std::cout << "Generating color image..." << std::endl;
		color_image ci = pc.to_color_image();
		
		std::cout << "Exporting color image..." << std::endl;
		ci.export_to_image_file("test.png");
		
		std::cout << "Done." << std::endl;
	}
};

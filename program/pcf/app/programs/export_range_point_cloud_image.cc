#include "../program.h"
#include <string>
#include <iostream>
#include <stdexcept>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(export_range_point_cloud_image) {
	void main() {
		range_point_cloud_full pc = ask_range_point_cloud();
		
		std::cout << "Generating color image..." << std::endl;
		color_image ci = pc.to_color_image(rgb_color::white);
		ci.flip(true, true);
		
		std::cout << "Exporting color image..." << std::endl;
		ci.export_to_image_file("test.png");
		
		std::cout << "Done." << std::endl;
	}
};

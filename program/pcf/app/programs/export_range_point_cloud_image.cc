#include "../program.h"
#include <string>
#include <iostream>
#include <stdexcept>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(export_range_point_cloud_image) {
	void main() {
		range_point_cloud_full pc = ask_range_point_cloud();
		
		{
		std::cout << "Generating color image..." << std::endl;
		color_image ci = pc.to_color_image(rgb_color::black);
		ci.flip(true, false);
		
		std::cout << "Exporting color image..." << std::endl;
		ci.export_to_image_file("color.png");
		}

		{
		std::cout << "Generating range image..." << std::endl;
		range_image ri = pc.to_range_image();
		ri.flip(true, false);
		
		std::cout << "Exporting range image..." << std::endl;
		ri.export_visualization_to_image_file("range.png");
		}

		
		std::cout << "Done." << std::endl;
	}
};

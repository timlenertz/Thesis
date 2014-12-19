#include "../program.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(reproject_range_image) {
	void main() {
		range_point_cloud_full pc = ask_range_point_cloud();

		projection_image_camera cam(
			pose(),
			projection_frustum::symmetric_perspective_fov(angle::degrees(60), angle::degrees(60)),
			800, 800
		);

		std::cout << "Reprojecting..." << std::endl;
		range_point_cloud<point_full, projection_image_camera> pc2(pc, cam);
		
		std::cout << "Generating range image..." << std::endl;
		range_image ri = pc2.to_range_image();
		ri.flip(true, false);

		std::cout << "Exporting range image..." << std::endl;
		ri.export_visualization_to_image_file("out.png");

		std::cout << "Done." << std::endl;
	}
};

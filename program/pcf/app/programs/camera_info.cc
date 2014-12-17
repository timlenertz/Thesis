#include "../program.h"
#include <iostream>
#include <memory>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(camera_info) {
	void main() {
		std::unique_ptr<projection_camera> cam = nullptr;
	
		access_viewer_([&cam](viewer& vw) {
			const projection_camera& c = vw->get_camera();
			cam.reset(new projection_camera(c));
		});
		
		std::cout << "View matrix:" << std::endl;
		std::cout << cam->get_pose().view_transformation().matrix() << std::endl;
	}
};

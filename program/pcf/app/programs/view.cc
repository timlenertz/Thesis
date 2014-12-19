#include "../program.h"
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(view) {
	void main() {
		int action;
		while(action = shell::read_choice("Action", { "Exit", "Add point cloud" }))
			if(action == 1) {
				unorganized_point_cloud_full pc = ask_point_cloud();

				std::cout << "Adding to scene..." << std::endl;
				access_viewer_([&pc](viewer& vw) {
					vw->add_point_cloud(pc);
					vw->add_bounding_box(pc.box());
				});
			}
				
		std::cout << "Done." << std::endl;
	}
};

#include "../program.h"
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(view, true) {
	void main() {
		int action;
		choices cs { "Exit", "Add point cloud" };
		while( (action = read_choice("Action", cs)) != 0 )
			if(action == 1) {
				unorganized_point_cloud_full pc = ask_point_cloud();

				std::cout << "Adding to scene..." << std::endl;
				shell::access_viewer([&pc](viewer& vw) {
					auto& spc = vw->add_point_cloud(pc);
					vw->add_bounding_box(pc.box()).set_parent(spc);
				});
			}
				
		std::cout << "Done." << std::endl;
	}
};

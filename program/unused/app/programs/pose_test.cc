#include "../program.h"
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(pose_test, false) {
	void main() {
		unorganized_point_cloud_full pc = ask_point_cloud();

		std::cout << "Adding to scene..." << std::endl;
		auto b = pc.box();
		shell::access_viewer([&pc, &b](viewer& vw) {
			#pragma omp parallel for
			for(std::ptrdiff_t x = -1; x <= 1; ++x) { pose p; p.position[0] = b.side_lengths()[0] * 2.5 * x;
			for(std::ptrdiff_t y = -1; y <= 1; ++y) { p.position[1] = b.side_lengths()[1] * 2.5 * y;
			for(std::ptrdiff_t z = -1; z <= 1; ++z) { p.position[2] = b.side_lengths()[2] * 2.5 * z;
				p.orientation =
					Eigen::AngleAxisf(random_real(-pi * 0.3, pi * 0.3), Eigen::Vector3f::UnitX()) *
					Eigen::AngleAxisf(random_real(-pi * 0.3, pi * 0.3), Eigen::Vector3f::UnitY()) *
					Eigen::AngleAxisf(random_real(-pi * 0.3, pi * 0.3), Eigen::Vector3f::UnitZ());
			
				auto& spc = vw->add_point_cloud(pc);
				spc.set_relative_pose(p);
				vw->add_bounding_box(b).set_parent(spc);
				
				std::cout << "Added (" << x << ", " << y << ", " << z << ")" << std::endl;
			}}}
		});
		
		std::cout << "Done." << std::endl;
	}
};

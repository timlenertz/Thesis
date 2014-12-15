#include "../program.h"
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "../../pcf/io/pointscan_importer.h"
#include "../../pcf/io/ply_importer.h"
#include "../../pcf/point_cloud/unorganized_point_cloud.h"
#include "../../pcf_viewer/scene/bounding_box.h"
#include "../../pcf_viewer/scene/frustum.h"

using namespace pcf;

PCF_PROGRAM(pose_test) {
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
		auto b = pc.box();
		access_viewer_([&pc, &b](viewer& vw) {
			pose p;
			for(std::ptrdiff_t x = -1; x <= 1; ++x) { p.position[0] = b.side_lengths()[0] * 2.5 * x;
			for(std::ptrdiff_t y = -1; y <= 1; ++y) { p.position[1] = b.side_lengths()[1] * 2.5 * y;
			for(std::ptrdiff_t z = -1; z <= 1; ++z) { p.position[2] = b.side_lengths()[2] * 2.5 * z;
				p.orientation =
					Eigen::AngleAxisf(random_real(-pi * 0.3, pi * 0.3), Eigen::Vector3f::UnitX()) *
					Eigen::AngleAxisf(random_real(-pi * 0.3, pi * 0.3), Eigen::Vector3f::UnitY()) *
					Eigen::AngleAxisf(random_real(-pi * 0.3, pi * 0.3), Eigen::Vector3f::UnitZ());
			
				vw->add_point_cloud(pc).set_pose(p);
				vw->add_bounding_box(b).set_pose(p);
				
				std::cout << "Added (" << x << ", " << y << ", " << z << ")" << std::endl;
			}}}
		});
		
		std::cout << "Done." << std::endl;
	}
};

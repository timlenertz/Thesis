#include "../program.h"
#include <Eigen/Geometry>
#include <iostream>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(icp_test) {
	void main() {
		unorganized_point_cloud_full loose = ask_point_cloud();
	
		Eigen::Affine3f actual_transformation(
			Eigen::AngleAxisf(0.1*M_PI, Eigen::Vector3f::UnitX())
		);
		
		std::cout << "Creating structured fixed point cloud..." << std::endl;
		//grid_point_cloud_full fixed(loose, 2.0);
		tree_point_cloud<kdtree_traits, point_xyz> fixed(loose, system_page_size / sizeof(point_full));
		loose.apply_transformation(actual_transformation);
		
		scene_point_cloud* scene_loose = nullptr;		
		std::cout << "Adding to scene..." << std::endl;
		access_viewer_([&loose, &fixed, &scene_loose](viewer& vw) {
			auto& scf = vw->add_point_cloud(fixed);
			set_unique_color(scf->begin(), scf->end(), rgb_color::white);
			
			auto& scl = vw->add_point_cloud(loose);
			set_unique_color(scl->begin(), scl->end(), rgb_color::red);
			
			scene_loose = &scl;
		});
		
		int i = 0;
		auto cor = make_closest_point_correspondences(
			fixed, loose,
			[&i](const point_full&) { return (++i % 500) == 0; },
			[](const point_full&, const point_full&) { return 1.0; }
		);
		
		shell::read_line("Ready");
		
		iterative_correspondences_registration<decltype(cor)> icp(fixed, loose, cor);
		
		std::cout << "Running ICP..." << std::endl;
		icp.run([&](const Eigen::Affine3f& trans, float err) {
			std::cout << "Error: " << err << std::endl;
			access_viewer_([&loose, &fixed, &scene_loose](viewer& vw) {
				scene_loose->set_pose(loose.absolute_pose());
			});
		});
		
		std::cout << "Done." << std::endl;
	}
};

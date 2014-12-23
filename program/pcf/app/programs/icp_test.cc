#include "../program.h"
#include <Eigen/Geometry>
#include <iostream>
#include <random>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(icp_test) {
	void main() {
		unorganized_point_cloud_full loose_orig = ask_point_cloud();
	
		Eigen::Affine3f actual_transformation(
			Eigen::AngleAxisf(0.1*M_PI, Eigen::Vector3f::UnitX())
		);
		
		unorganized_point_cloud_full loose(loose_orig, loose_orig.size()*2);
		
		loose.downsample_random(0.5);
		loose_orig.downsample_random(0.5);
				
		std::cout << "Creating structured fixed point cloud..." << std::endl;
		tree_point_cloud<kdtree_traits, point_xyz> fixed(loose_orig, system_page_size / sizeof(point_full));
		
		loose.apply_transformation(actual_transformation);	
		loose.add_random_noise_around_points<>(loose.size() / 3, std::normal_distribution<float>(0.0, 0.5));

		
		scene_point_cloud* scene_loose = nullptr;		
		std::cout << "Adding to scene..." << std::endl;
		shell::access_viewer([&loose, &fixed, &scene_loose](viewer& vw) {
			auto& scf = vw->add_point_cloud(fixed);
			set_unique_color(scf->begin(), scf->end(), rgb_color::white);
			
			auto& scl = vw->add_point_cloud(loose);
			set_unique_color(scl->begin(), scl->end(), rgb_color::red);
			
			scene_loose = &scl;
		});
				
		auto cor = make_closest_point_correspondences(
			fixed, loose,
			skip_point_filter(100),
			[](const point_full&, const point_full&) { return 1.0; }
		);
		
		shell::read_line("Ready");
		
		iterative_correspondences_registration<decltype(cor)> icp(fixed, loose, cor);
		
		std::cout << "Running ICP..." << std::endl;
		icp.run([&](const Eigen::Affine3f& trans, float err) {
			std::cout << "Error: " << err << std::endl;
			shell::access_viewer([&loose, &fixed, &scene_loose](viewer& vw) {
				scene_loose->set_relative_pose(loose.relative_pose());
			});
		});
		
		std::cout << "Done." << std::endl;
	}
};

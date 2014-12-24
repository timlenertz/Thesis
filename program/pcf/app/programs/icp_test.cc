#include "../program.h"
#include <Eigen/Geometry>
#include <iostream>
#include <random>

#include "../lib.h"

using namespace pcf;

PCF_PROGRAM(icp_test, true) {
	void main() {
		unorganized_point_cloud_full fixed_orig = ask_point_cloud();
		bounding_box b = fixed_orig.box();
		
		unorganized_point_cloud_full loose = fixed_orig;

		float div = (b.extremity[0] - b.origin[0]);

		fixed_orig.downsample_random_field([&](const Eigen::Vector3f& p)->float {
			return (b.extremity[0] - p[0]) / div;
		});

		loose.downsample_random_field([&](const Eigen::Vector3f& p)->float {
			return (p[0] - b.origin[0]) / div;
		});


		Eigen::Affine3f actual_transformation(
			Eigen::AngleAxisf(0.03*M_PI, Eigen::Vector3f::UnitX()) *
			Eigen::AngleAxisf(0.04*M_PI, Eigen::Vector3f::UnitY()) *
			Eigen::AngleAxisf(0.05*M_PI, Eigen::Vector3f::UnitZ()) *
			Eigen::Translation3f(Eigen::Vector3f(1.0, -1.0, 0.5) * 10.0)
		);
				
		//range_image_camera cam(pose(), {angle(-pi),angle(+pi)}, {angle(-pi),angle(+pi)}, 800, 800);
		//loose.erase_invisible_points(cam);
					
		std::cout << "Creating structured fixed point cloud..." << std::endl;
		tree_point_cloud<kdtree_traits, point_xyz> fixed(fixed_orig, system_page_size / sizeof(point_full));
		
		loose.apply_transformation(actual_transformation);
		
		
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

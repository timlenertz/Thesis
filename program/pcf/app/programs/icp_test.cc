#include "../program.h"
#include <Eigen/Geometry>
#include <iostream>
#include "../../pcf/pcf.h"
#include "lib.h"
#include "../../pcf_viewer/scene/point_cloud.h"

using namespace pcf;

PCF_PROGRAM(icp_test) {
	void main() {
		unorganized_point_cloud_full loose = ask_point_cloud();
	
		Eigen::Affine3f actual_transformation(
			Eigen::AngleAxisf(0.1*M_PI, Eigen::Vector3f::UnitX())
		);
		
		std::cout << "Creating structured fixed point cloud..." << std::endl;
		grid_point_cloud_full fixed(loose, 2.0);
		loose.apply_transformation(actual_transformation);
		
		scene_point_cloud* scene_loose = nullptr;		
		std::cout << "Adding to scene..." << std::endl;
		access_viewer_([&loose, &fixed, &scene_loose](viewer& vw) {
			auto& scf = vw->add_point_cloud(fixed);
			set_unique_color(scf->begin(), scf->end(), rgb_color::yellow);
			
			auto& scl = vw->add_point_cloud(loose);
			set_unique_color(scl->begin(), scl->end(), rgb_color::green);
			
			scene_loose = &scl;
		});
		
		auto cor = make_closest_point_correspondences(
			fixed, loose,
			[](const point_full&) { return true; },
			[](const point_full&, const point_full&) { return 1.0; }
		);
		
		iterative_correspondences_registration<decltype(cor)> icp(fixed, loose, cor);
		
		for(;;) {
			icp.iteration();
			loose.absolute_pose.transform( icp.last_estimated_transformation().inverse() );
			
			access_viewer_([&loose, &fixed, &scene_loose](viewer& vw) {
				scene_loose->set_pose(loose.absolute_pose);
			});
		}
	}
};

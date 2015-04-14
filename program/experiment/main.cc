#include <cstdlib>
#include <iostream>

#include <pcf/core.h>
#include <pcf/viewer.h>

using namespace pcf;
using namespace Eigen;

range_image frontal_view(const point_cloud_full& pc) {
	plane r = fit_plane_to_points(pc.begin(), pc.end());
	
	pose ps;
	ps.position = r.project(pc.center_of_mass());
	ps.orientation.setFromTwoVectors(r.normal, Eigen::Vector3f::UnitZ());
	projection_image_camera cam(ps, projection_bounding_box::symmetric_orthogonal(5, 5), 600, 600);
	
	return project(pc, cam).to_range_image();
}


auto correspondences(const kdtree_point_cloud_full& P, const kdtree_point_cloud_full& Q) {
	return make_closest_point_correspondences(P, Q);
}

int main(int argc, const char* argv[]) {
	kdtree_point_cloud_full hi = import_point_cloud("data/hi.ply");
	kdtree_point_cloud_full lo = import_point_cloud("data/lo.ply");

	plane r = fit_plane_to_points(hi.begin(), hi.end());

	auto cor = correspondences(hi, lo);
	
	frontal_view(hi).export_visualization_to_image_file("lo.png");
	frontal_view(lo).export_visualization_to_image_file("hi.png");
}

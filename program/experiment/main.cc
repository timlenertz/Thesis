#include <cstdlib>
#include <iostream>
#include <random>
#include <cmath>

//#include <pcf/core.h>
#include "../pcf/pcf_core/pcf_core.h"

using namespace pcf;
using namespace Eigen;

color_image make_view(point_cloud_full& P, point_cloud_full& Q) {
	projection_image_camera cam(pose::from_string("-0.00391672,-4.38406,4.33067,0.935426,0.353468,-0.00587757,0.00222094"), projection_frustum::symmetric_perspective_fov_x(angle::degrees(60), 1), 600, 600);
	camera_range_point_cloud_full<projection_image_camera> rpc(cam);
	rpc.project(P, rgb_color::red);
	rpc.project(Q);
	color_image im = rpc.to_color_image();
	im.flip(true, false);
	return im;
}

projection_image_camera orthogonal_projection_camera(const plane& A, std::size_t res) {	
	pose ps;
	ps.position = Eigen::Vector3f::Zero();
	ps.orientation.setFromTwoVectors(A.normal, Eigen::Vector3f::UnitZ());
	return projection_image_camera(ps, projection_bounding_box::symmetric_orthogonal(6, 6), res, res);
}


int seed = 1;

auto hi_point_cloud() {
	grid_point_cloud_full gpc = import_point_cloud("data/hi.ply");
	set_unique_color(gpc.begin(), gpc.end(), 0xffffff);
	//compute_normals(gpc);
	return gpc;

	unorganized_point_cloud_full pc = make_relief_point_cloud(5.0, 1000, seed);
	projection_image_camera cam(
		pose::from_string("0.220013,-4.92678,3.35604,0.912652,0.408613,0.00823912,0.00583399"),
		projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 1.0),
		400, 400
	);
	pc.downsample_projection(cam);
	//return pc;
}

auto lo_point_cloud() {
	grid_point_cloud_full gpc = import_point_cloud("data/lo.ply");
	set_unique_color(gpc.begin(), gpc.end(), 0xffffff);
	//compute_normals(gpc);
	return gpc;

	unorganized_point_cloud_full pc = make_relief_point_cloud(5.0, 1000, seed);
	projection_image_camera cam(
		pose::from_string("-0.0192693,-5.58523,3.69624,0.899365,0.437096,-0.00847658,0.00411958"),
		projection_frustum::symmetric_perspective_fov_x(angle::degrees(60.0), 1.0),
		200, 200
	);
	pc.downsample_projection(cam);
	pc.randomly_displace_points(std::uniform_real_distribution<float>(-0.01, 0.01));
	pc.rotate_x_axis(angle::degrees(20));
	pc.rotate_y_axis(angle::degrees(10));
	pc.rotate_z_axis(angle::degrees(5));
	pc.move_x(0.2);
	pc.apply_pose();
	//return pc;
}


int main(int argc, const char* argv[]) {
	if(argc >= 2) seed = std::atoi(argv[1]);

	std::cout << "Loading point clouds" << std::endl;
	auto P = hi_point_cloud();
	auto Q = lo_point_cloud();
	
	make_view(P, Q).export_to_image_file("output/initial.png");

	std::cout << "Fitting plane A" << std::endl;
	plane A;
	{
		auto sum = P + Q;
		A = fit_plane_to_points(sum.begin(), sum.end());
	}
	
	std::cout << "Fitting planes A_P and A_Q" << std::endl;
	plane A_P, A_Q;
	A_P = fit_plane_to_points(P.begin(), P.end());
	A_Q = fit_plane_to_points(Q.begin(), Q.end());
	

	std::cout << "Moving onto A" << std::endl;
	{
		P.transform( Quaternionf::FromTwoVectors(A_P.normal, A.normal) );
		Q.transform( Quaternionf::FromTwoVectors(A_Q.normal, A.normal) );
	}
	
	make_view(P, Q).export_to_image_file("output/moved.png");
	
	std::cout << "Making projection images" << std::endl;
	auto cam = orthogonal_projection_camera(A, 1000);
	project(P, cam).to_color_image().export_to_image_file("output/P.png");
	project(Q, cam).to_color_image().export_to_image_file("output/Q.png");
}

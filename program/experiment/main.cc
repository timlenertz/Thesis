#include <cstdlib>
#include <iostream>

//#include <pcf/core.h>
#include "../pcf/pcf_core/pcf_core.h"

using namespace pcf;
using namespace Eigen;

color_image make_view(point_cloud_full& P, point_cloud_full& Q) {
	projection_image_camera cam(pose::from_string("-0.00391672,-4.38406,4.33067,0.935426,0.353468,-0.00587757,0.00222094"), projection_frustum::symmetric_perspective_fov_x(angle::degrees(60), 1), 400, 400);
	camera_range_point_cloud_full<projection_image_camera> rpc(cam);
	rpc.project(P, rgb_color::red);
	rpc.project(Q);
	color_image im = rpc.to_color_image();
	im.flip(true, false);
	return im;
}

int seed = 1;

auto hi_point_cloud() {
	auto pc = make_relief_point_cloud(5.0, 1000, seed);
	return pc;
	//grid_point_cloud_full gpc = import_point_cloud("data/hi.ply");
	//compute_normals(gpc);
	//return gpc;
}

auto lo_point_cloud() {
	unorganized_point_cloud_full upc = hi_point_cloud();
	//grid_point_cloud_full gpc = import_point_cloud("data/lo.ply");
	//compute_normals(gpc);
	//unorganized_point_cloud_full upc = std::move(gpc);
	upc.rotate_x_axis(angle::degrees(20));
	upc.rotate_y_axis(angle::degrees(10));
	upc.apply_pose();
	return upc;
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
	
	/*for(std::ptrdiff_t i = 0; i < P.size()/3; i += 3) {
		P[i] = point_full( A_P.normal * random_real(0.0f, 10.0f) );
		Q[i] = point_full( A_Q.normal * random_real(0.0f, 10.0f) );
	}
	make_view(P, Q).export_to_image_file("output/initial.png");
	*/

	std::cout << "Moving onto A" << std::endl;
	{
		P.transform( Quaternionf::FromTwoVectors(A_P.normal, A.normal) );
		Q.transform( Quaternionf::FromTwoVectors(A_Q.normal, A.normal) );
	}
	
	make_view(P, Q).export_to_image_file("output/moved.png");
}

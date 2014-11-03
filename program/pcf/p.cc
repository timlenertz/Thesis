#include <iostream>

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "pcf/io/ply_reader.h"
#include "pcf/point_cloud/range_point_cloud.h"
#include "pcf/range_image.h"

using namespace pcf;

point_cloud_xyz bunny_model() {
	ply_reader ply("../bunny.ply");
	return point_cloud_xyz::create_from_reader(ply);
}


int main(int argc, const char* argv[]) {
	pose p;
	p.position = Eigen::Vector3f(0.0, 0.1, 0.5);

	camera cam(p, 30.0, 0.1, 1000.0, 640, 480);

	range_point_cloud_xyz pc(bunny_model(), cam);
	
	range_image ri = pc.to_range_image();
	ri.save_image("test.png");
}
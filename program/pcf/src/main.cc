#include <iostream>
#include <stdexcept>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include "point_cloud/point_cloud.h"
#include "point_cloud/range_point_cloud.h"
#include "util/projection.h"
#include "registration/point_correspondences.h"
#include "range_image.h"

#include "util/mmap_allocator.h"
#include "io/ply_reader.h"
#include "io/ply_writer.h"
#include "point.h"

using namespace pcf;

int main(int argc, const char* argv[]) try {
	ply_reader ply(argv[1]);
	point_cloud<point_xyz> pc(ply.size());
	ply.read(pc.data(), ply.size());

	auto proj = perspective_projection_matrix(60, 4.0/3.0, 10000, 0.1) * Eigen::Translation<float, 3>(0, 0.1, 1) * Eigen::AngleAxisf(M_PI, Eigen::Vector3f::UnitZ());
	
	
	range_point_cloud<point_xyz> rpc(400, 300);
	rpc.project_point_cloud(pc, proj);

	range_image ri = rpc.to_range_image();
	ri.save_image("test.png");
	
	
} catch(const std::exception& ex) {
	std::cerr << "Uncaught exception: " << ex.what() << std::endl;
}



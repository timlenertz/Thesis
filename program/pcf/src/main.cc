#include <iostream>
#include <stdexcept>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include "point_cloud/point_cloud.h"
#include "point_cloud/range_point_cloud.h"
#include "util/projection.h"

#include "util/mmap_allocator.h"
#include "io/ply_reader.h"
#include "io/ply_writer.h"
#include "point.h"

using namespace pcf;

int main(int argc, const char* argv[]) try {
	ply_reader ply(argv[1]);
	point_cloud<point_xyz> pc(ply.size());
	ply.read(pc.data(), ply.size());

	auto proj = perspective_projection_matrix(90, 4.0/3.0, 1000, 0.1) * Eigen::Translation<float, 3>(0, 0, std::strtof(argv[2], nullptr));
	range_point_cloud<point_xyz> rpc(50, 20);
	
	rpc.project_point_cloud(pc, proj);
	
	rpc.print();
	rpc.apply_transformation( Eigen::Translation<float, 3>(0, 0, 1000) );
	std::cout << "-------------------------------------------" << std::endl;
	rpc.print();


} catch(const std::exception& ex) {
	std::cerr << "Uncaught exception: " << ex.what() << std::endl;
}

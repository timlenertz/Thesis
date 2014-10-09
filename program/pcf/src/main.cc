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

	auto proj = perspective_projection_matrix(60, 4.0/3.0, 1000, 0.1) * Eigen::Affine3f(Eigen::Translation3f(0.0, 0.0, -100.0));
	range_point_cloud<point_xyz> rpc(400, 300, proj.matrix());
	rpc.erase();
	rpc.project_point_cloud(pc, proj.matrix());
		
	rpc.remove_invalid_points();
	std::cout << rpc.size() << std::endl;
	
	ply_writer<point_xyz> plyw(argv[2]);
	plyw.write(rpc.data(), rpc.size());

} catch(const std::exception& ex) {
	std::cerr << "Uncaught exception: " << ex.what() << std::endl;
}


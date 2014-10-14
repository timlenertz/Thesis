#include <iostream>
#include <stdexcept>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include "point_cloud/point_cloud.h"
#include "point_cloud/range_point_cloud.h"
#include "util/projection.h"
#include "registration/points_correspondence.h"
#include "registration/icp.h"

#include "util/mmap_allocator.h"
#include "io/ply_reader.h"
#include "io/ply_writer.h"
#include "point.h"

using namespace pcf;

static void export_pc(const std::string& path, const point_cloud<point_xyz>& pc) {
	ply_writer<point_xyz> ply(path);
	pc.write(ply);
}

int main(int argc, const char* argv[]) try {
	using pc_t = point_cloud<point_xyz>;
	using rpc_t = range_point_cloud<point_xyz>;

	ply_reader ply(argv[1]);
	pc_t pc_og = pc_t::create_from_reader(ply);
	
	rpc_t rpc(400, 300);
	rpc.project_point_cloud(pc_og, perspective_projection_matrix(60, 4.0/3.0, 0.1, 10000));

	pc_t pc2 = rpc;
	pc2.erase_invalid_points();
	
	std::cout << "Building points correspondence" << std::endl;
	points_correspondence<pc_t, rpc_t> cor;
	for(std::ptrdiff_t i = 0; i <= pc2.size(); ++i) {
		cor.add(pc2[i], rpc[i]);
	}
	
	Eigen::Affine3f T(
		Eigen::AngleAxisf(0.01*M_PI, Eigen::Vector3f::UnitY()) * Eigen::Translation3f(0.0, 1.0, 0.0)
	);
	std::cout << "Real transformation:" << std::endl;
	std::cout << T.matrix() << std::endl;


	pc2.apply_transformation(T);
	std::cout << "After trans: " << cor.error(euclidian_distance_sq) << std::endl;

	icp<pc_t, rpc_t> ic(pc2, rpc);
	ic();
	
	std::cout << "After ICP: " << cor.error(euclidian_distance_sq) << std::endl;

} catch(const std::exception& ex) {
	std::cerr << "Uncaught exception: " << ex.what() << std::endl;
}



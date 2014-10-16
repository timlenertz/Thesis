#include <iostream>
#include <stdexcept>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include "point_cloud/point_cloud.h"
#include "point_cloud/range_point_cloud.h"
#include "util/projection.h"
#include "registration/point_correspondences.h"
#include "registration/closest_point_correspondences.h"
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

	ply_reader ply(argv[1]);
	pc_t pc_high = pc_t::create_from_reader(ply);
	pc_t pc_low = pc_high;
	pc_low.downsample_random(0.1);
	
	std::cout << pc_high.size() << std::endl;
	std::cout << pc_low.size() << std::endl;

	closest_point_correspondences<pc_t, pc_t> cor(pc_high, pc_low);
	cor.compute();	

} catch(const std::exception& ex) {
	std::cerr << "Uncaught exception: " << ex.what() << std::endl;
}



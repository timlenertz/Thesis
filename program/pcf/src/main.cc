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

	ply_reader ply(argv[1]);
	pc_t pc = pc_t::create_from_reader(ply);
	
	std::cout << pc.size() << std::endl;
	pc.downsample_random(0.5);
	std::cout << pc.size() << std::endl;
	

} catch(const std::exception& ex) {
	std::cerr << "Uncaught exception: " << ex.what() << std::endl;
}



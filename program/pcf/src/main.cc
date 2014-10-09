#include <iostream>
#include <stdexcept>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include "point_cloud.h"
#include "util/mmap_allocator.h"
#include "io/ply_reader.h"
#include "io/ply_writer.h"
#include "point.h"

using namespace pcf;

int main(int argc, const char* argv[]) try {
	ply_reader ply(argv[1]);
	point_cloud<point_xyz> pc1(ply.size());
	point_cloud<point_xyz> pc2(ply.size());
	
	ply.read(pc1.data(), ply.size());
	ply.rewind();
	ply.read(pc2.data(), ply.size());

	pc1.apply_transformation( Eigen::AngleAxisf(0.1 * M_PI, Eigen::Vector3f::UnitX()) );

	for(auto& p1 : pc1) {
		auto& p2 = pc2.find_closest_point(p1, euclidian_distance_sq);
		std::cout << p1 << " --> " << p2 << std::endl;
	}

} catch(const std::exception& ex) {
	std::cerr << "Uncaught exception: " << ex.what() << std::endl;
}
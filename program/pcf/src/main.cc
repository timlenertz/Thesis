#include <iostream>
#include <stdexcept>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include "point_cloud/point_cloud.h"
#include "point_cloud/range_point_cloud.h"
#include "util/projection.h"
#include "registration/point_correspondences.h"

#include "util/mmap_allocator.h"
#include "io/ply_reader.h"
#include "io/ply_writer.h"
#include "point.h"

using namespace pcf;

int main(int argc, const char* argv[]) try {
	ply_reader ply(argv[1]);
	point_cloud<point_xyz> pc1(ply.size());
	ply.read(pc1.data(), ply.size());

	point_cloud<point_xyz> pc2 = pc1;
	
	pc2.apply_transformation(Eigen::AngleAxisf(0.1*M_PI, Eigen::Vector3f::UnitX()));
	
	point_correspondences<point_xyz, point_xyz> cor;
	for(;;) {
		cor.find_closest_points(pc1, pc2);
		std::cout << cor.error() << std::endl;

		cor.erase();
		pc2.apply_transformation(Eigen::AngleAxisf(-0.01*M_PI, Eigen::Vector3f::UnitX()));
	}

} catch(const std::exception& ex) {
	std::cerr << "Uncaught exception: " << ex.what() << std::endl;
}

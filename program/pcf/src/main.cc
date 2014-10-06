#include <iostream>
#include <Eigen/Eigen>

#include "point_cloud_mmap.h"
#include "point_cloud_alloc.h"
#include "ply_reader.h"
#include "point.h"

using namespace pcf;

int main(int argc, const char* argv[]) {
	ply_reader ply(argv[1]);
	std::size_t sz = ply.size();
	
	std::cout << "Size: " << sz << std::endl;
		
	point_cloud_mmap<point_xyz> pc(sz, "pc.dat");
	ply.read(pc.data(), sz);
	
	std::cout << "Imported point cloud." << std::endl;
	
	using transform_t = Eigen::Transform<float, 3, Eigen::Affine>;
	transform_t t = Eigen::AngleAxisf(0.01*M_PI, Eigen::Vector3f::UnitX()) * Eigen::Translation3f(0.1, 0.1, 0.1);

	std::cout << t.matrix() << std::endl;
	for(std::ptrdiff_t i = 0; i < 10000; ++i) {
		std::cout << pc[1] << std::endl;

		pc.apply_transformation(t);
	}	
}
#include <iostream>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include "point_cloud_alloc.h"
#include "ply_reader.h"
#include "point.h"

using namespace pcf;

int main(int argc, const char* argv[]) {
	ply_reader ply(argv[1], '\r');
	std::size_t sz = ply.size();
	
	std::cout << "Size: " << sz << std::endl;
	
	point_cloud_alloc<point_xyz> pc(sz);
	ply.read(pc.data(), sz);
	
	std::cout << "Imported point cloud." << std::endl;
	
	using transform_t = Eigen::Transform<float, 3, Eigen::Affine>;
	transform_t t( Eigen::AngleAxisf(0.01*M_PI, Eigen::Vector3f::UnitX()) );

	std::cout << t.matrix() << std::endl;
	
	for(;;) {
		std::cout << pc[0] << std::endl;

		pc.apply_transformation(t);
		std::cout << "Transformation applied." << std::endl;		
	}
}
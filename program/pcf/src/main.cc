#include <iostream>
#include <Eigen/Eigen>

#include "point_cloud_mmap.h"
#include "point_cloud_alloc.h"
#include "ply_reader.h"
#include "ply_writer.h"
#include "point.h"

using namespace pcf;

int main(int argc, const char* argv[]) {
	ply_reader ply_in(argv[1]);
	std::size_t sz = ply_in.size();
	std::cout << "Size: " << sz << std::endl;
	
	std::cout << "Importing PLY..." << std::endl;
	point_cloud_mmap<point_xyz> pc(sz, "pc.dat");
	ply_in.read(pc.data(), sz);
	std::cout << "Done." << std::endl;
	
	std::cout << "Applying transformation..." << std::endl;
	using transform_t = Eigen::Transform<float, 3, Eigen::Affine>;
	transform_t t = Eigen::AngleAxisf(0.01*M_PI, Eigen::Vector3f::UnitX()) * Eigen::Translation3f(0.1, 0.1, 0.1);
	pc.apply_transformation(t);
	std::cout << "Done." << std::endl;


	ply_writer<point_xyz> ply_out(argv[2]);
	std::cout << "Exporting PLY..." << std::endl;
	ply_out.write(pc.data(), sz);
	std::cout << "Done." << std::endl;
}
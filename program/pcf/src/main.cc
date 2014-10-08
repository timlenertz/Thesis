#include <iostream>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include "point_cloud_mmap.h"
#include "point_cloud_alloc.h"
#include "io/ply_reader.h"
#include "io/ply_writer.h"
#include "point.h"

using namespace pcf;

int main(int argc, const char* argv[]) {
	ply_reader ply(argv[1]);
	point_cloud_alloc<point_xyz> pc(ply.size());
	ply.read(pc.data(), ply.size());

	pc.apply_transformation(Eigen::Translation3f(-pc.mean()));

	std::cout << pc.mean() << std::endl;
}
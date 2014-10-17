#include <iostream>
#include <stdexcept>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include "point_cloud/point_cloud.h"
#include "point_cloud/octree_point_cloud.h"
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
	using cloud = point_cloud<point_xyz>;
	using ocloud = octree_point_cloud<point_xyz>;

	ply_reader ply(argv[1]);
	std::cout << "Loading from PLY file" << std::endl;
	cloud pc1 = cloud::create_from_reader(ply);
	std::cout << "Copying" << std::endl;
	cloud pc2 = pc1;
	std::cout << "Transforming" << std::endl;
	pc2.apply_transformation( Eigen::AngleAxisf(0.05*M_PI, Eigen::Vector3f::UnitY()) );

	std::cout << "Finding closest points (Unstructured)" << std::endl;
	closest_point_correspondences<cloud, cloud> cor(pc1, pc2);
	cor.compute();
	
	std::cout << "Building Octree 1" << std::endl;
	ocloud opc1(std::move(pc1), 100);
	std::cout << "Building Octree 2" << std::endl;
	ocloud opc2(std::move(pc2), 100);
	
	std::cout << "Finding closest points (Octree)" << std::endl;
	closest_point_correspondences<ocloud, ocloud> ocor(opc1, opc2);
	ocor.compute();

	
	export_pc("opc2.ply", pc2);
	
} catch(const std::exception& ex) {
	std::cerr << "Uncaught exception: " << ex.what() << std::endl;
}



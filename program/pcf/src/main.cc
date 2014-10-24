#include <iostream>
#include <stdexcept>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include "point_cloud/point_cloud.h"
#include "point_cloud/tree_point_cloud.h"
#include "point_cloud/tree_traits/octree_traits.h"
#include "point_cloud/tree_traits/kdtree_traits.h"
#include "point_cloud/tree_traits/null_tree_traits.h"
#include "point_cloud/grid_point_cloud.h"
#include "util/projection.h"
#include "registration/point_correspondences.h"
#include "registration/closest_point_correspondences.h"
#include "registration/icp.h"

#include "util/mmap_allocator.h"
#include "io/ply_reader.h"
#include "io/ply_writer.h"
#include <sstream>
#include "point.h"

using namespace pcf;

template<typename Cloud>
static void export_pc(const std::string& path, const Cloud& pc) {
	ply_writer<typename Cloud::point_type> ply(path);
	pc.write(ply);
}

int main(int argc, const char* argv[]) try {
	std::size_t cap = 10000;

	using pt = point_full;
	using cloud  = point_cloud<point_full>;
	using ocloud = tree_point_cloud<octree_traits, point_full>;
	using kcloud = tree_point_cloud<kdtree_traits, point_full>;
	using gcloud = grid_point_cloud<point_full>;

	ply_reader ply(argv[1]);
	std::cout << "Loading from PLY file" << std::endl;
	cloud pc1 = cloud::create_from_reader(ply);
	
	auto cub = pc1.bounding_cuboid();
	std::cout << cub << std::endl;
	
	std::cout << "Copying" << std::endl;
	cloud pc2 = pc1;
	std::cout << "Transforming" << std::endl;
	pc2.apply_transformation( Eigen::AngleAxisf(0.05*M_PI, Eigen::Vector3f::UnitY()) );

	//std::cout << "Finding closest points (Unstructured)" << std::endl;
	//closest_point_correspondences<cloud, cloud> cor(pc1, pc2);
	//cor.compute();

	std::cout << "Building Octree" << std::endl;
	ocloud opc1(std::move(pc1), cap);
	export_pc("o.ply", opc1);
	std::cout << "Verifying..." << std::endl;
	std::cout << opc1.verify() << std::endl;
	
	opc1.test_ascend(opc1[0]);
	
	/*std::cout << "Finding closest points (Octree)" << std::endl;
	closest_point_correspondences<ocloud, cloud> ocor(opc1, pc2);
	ocor.compute();
*/
	
	std::cout << "Building Kdtree" << std::endl;
	kcloud kpc1(std::move(opc1), cap);
	export_pc("k.ply", kpc1);
	std::cout << "Verifying..." << std::endl;
	std::cout << kpc1.verify() << std::endl;

/*
	std::cout << "Finding closest points (Kdtree)" << std::endl;
	closest_point_correspondences<kcloud, cloud> kcor(kpc1, pc2);
	kcor.compute();*/


	std::cout << "Building Grid" << std::endl;
	float c = cub.side_lengths()[0] / 20.0;
	gcloud gpc1(std::move(kpc1), Eigen::Vector3f(c, c, c));
	export_pc("g.ply", gpc1);
	std::cout << "Verifying..." << std::endl;
	std::cout << gpc1.verify() << std::endl;


	std::cout << "Finding closest points (Grid)" << std::endl;
	closest_point_correspondences<gcloud, cloud> gcor(gpc1, pc2);
	gcor.compute();

	
} catch(const std::exception& ex) {
	std::cerr << "Uncaught exception: " << ex.what() << std::endl;
}



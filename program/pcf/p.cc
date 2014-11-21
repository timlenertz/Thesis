#include <iostream>

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "pcf/util/random.h"
#include "pcf/util/mmap_allocator.h"
#include "pcf/io/ply_importer.h"
#include "pcf/io/ply_exporter.h"
#include "pcf/io/pointscan_importer.h"
#include "pcf/point_cloud/point_cloud.h"
#include "pcf/point_cloud/unorganized_point_cloud.h"
#include "pcf/point_cloud/tree/tree_point_cloud.h"
#include "pcf/point_cloud/tree/octree_traits.h"
#include "pcf/point_cloud/tree/kdtree_traits.h"
#include "pcf/point_cloud/grid/grid_point_cloud.h"
#include "pcf/registration/iterative_correspondences_registration.h"
#include "pcf/registration/correspondences/closest_point_correspondences.h"
#include "pcf/registration/correspondences/same_point_correspondences.h"
#include "pcf/registration/error_metric/mean_square_error.h"
#include "pcf/registration/transformation_estimation/svd_transformation_estimation.h"

using namespace pcf;


unorganized_point_cloud_xyz load(const char* filename) {
	ply_importer ply(filename);
	unorganized_point_cloud_xyz pc(ply);
	return pc;
}

void save(const point_cloud_xyz& pc, const char* filename) {
	ply_exporter ply(filename);
	pc.export_with(ply);
}

float reg_error(const point_cloud_xyz& l, const point_cloud_xyz& f) {
	same_point_correspondences<point_cloud_xyz, point_cloud_xyz> cor(l, f);
	mean_square_error err;
	cor(err);
	return err();
}

int main(int argc, const char* argv[]) {
	// Load fixed (untransformed) into grid pc
	std::cout << "Building fixed..." << std::endl;
	using gpc = grid_point_cloud<point_xyz>;
	gpc fixed(load(argv[1]), 10.0);
	//tree_point_cloud<kdtree_traits, point_xyz> fixed(load(argv[1]), 100);
	//point_cloud_xyz fixed = load(argv[1]);

	save(fixed, "fixed.ply");


	// Create and save transformed loose
	unorganized_point_cloud_xyz loose(fixed, false);

	std::cout << "No error: " << reg_error(fixed, loose) << std::endl;

	std::cout << "Initial transform..." << std::endl;
	Eigen::Affine3f trans(
		Eigen::AngleAxisf(0.07*M_PI, Eigen::Vector3f::UnitX()) *
		Eigen::AngleAxisf(-0.03*M_PI, Eigen::Vector3f::UnitY()) *
		Eigen::Translation3f( Eigen::Vector3f(0.02, 0.005, -0.01) )
	);
	loose.apply_transformation(trans);
	loose.downsample_grid(2.0, true, true);

	save(loose, "loose.ply");

	
	std::cout << "Initial error: " << reg_error(fixed, loose) << std::endl;
	
	// Run ICP
	auto cor = make_closest_point_correspondences(
		fixed, loose,
		[](const point_xyz&) { return true; },
		[](const point_xyz& a, const point_xyz& b)->float { return 1.0; }
	);
	cor.accepting_distance = 0.2;
	cor.rejecting_distance = 4;

	iterative_correspondences_registration<point_cloud_xyz, unorganized_point_cloud_xyz, decltype(cor)> reg(fixed, loose, cor);
	
	std::cout << "ICP..." << std::endl;
	reg.run();
	
	// Save registered loose
	save(loose, "registered.ply");
	
	std::cout << "Final error: " << reg_error(fixed, loose) << std::endl;
	

	
	
/*
	pose p;
	p.position = Eigen::Vector3f(-0.01, 0.1, -200.0);
	p.orientation = Eigen::AngleAxisf(M_PI, Eigen::Vector3f::UnitZ());

	camera cam(p, angle::degrees(60.0), angle::degrees(40.0));

	range_point_cloud_xyz pc(model(), cam, angle::degrees(0.1), angle::degrees(0.1));
	
	ply_writer<point_xyz> ply1("proj.ply");
	pc.write(ply1);


	
	range_image ri = pc.to_range_image();
	ri.save_image("test.png");

	range_point_cloud_xyz pc2(ri, cam);
	
	ply_writer<point_xyz> ply("revproj.ply");
	pc2.write(ply);


	
	
	int k = 100;
	int n = 0;
	float csz = optimal_grid_cell_length_for_knn(model(), k, 2.0);
	std::cout << "cell length: " << csz << std::endl;

	grid_point_cloud_full pc(model(), csz);
	for(auto& p : pc) p.color = 0xffffff;
	pc.find_nearest_neighbors(k, [&n](const auto&)->bool {
		return !(++n % 500);
	}, [k](const auto& p, const auto& nn) {

		int i = 0;
		for(const auto& p2 : nn) {
			if(++i == k) return;
			p2->color = 0x000000;
		}
	}, 0);
	
	ply_writer<point_full> ply("out.ply");
	pc.write(ply);*/
}

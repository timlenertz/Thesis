#include <iostream>

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "pcf/io/ply_reader.h"
#include "pcf/io/ply_reader.h"
#include "pcf/point_cloud/point_cloud.h"
#include "pcf/registration/iterative_correspondences_registration.h"
#include "pcf/registration/correspondences/closest_point_correspondences.h"
#include "pcf/registration/transformation_estimation/svd_transformation_estimation.h"

using namespace pcf;


point_cloud_xyz model(const char* filename) {
	ply_reader ply(filename);
	return point_cloud_xyz::create_from_reader(ply);
}


int main(int argc, const char* argv[]) {	
	point_cloud_xyz fixed = model(argv[1]);
	point_cloud_xyz loose = model(argv[2]);
	
	auto cor = make_closest_point_correspondences(
		fixed, loose,
		[](const point_xyz&) { return true; },
		[](const point_xyz&, const point_xyz&)->float { return 1.0; }
	);
	
	iterative_correspondences_registration<point_cloud_xyz, point_cloud_xyz, decltype(cor)> reg(fixed, loose, cor);
		
	reg.run();
	
	
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

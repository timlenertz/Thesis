#include <iostream>

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include "pcf/io/ply_reader.h"
#include "pcf/io/ply_writer.h"
#include "pcf/point_cloud/range_point_cloud.h"
#include "pcf/point_cloud/grid_point_cloud.h"
#include "pcf/range_image.h"
#include "pcf/util/random.h"

using namespace pcf;

static const char* filename;

point_cloud_xyz model() {
	ply_reader ply(filename);
	return point_cloud_xyz::create_from_reader(ply);
}


int main(int argc, const char* argv[]) {
/*
	filename = argv[1];

	pose p;
	p.position = Eigen::Vector3f(-0.01, 0.1, -0.5);
	p.orientation = Eigen::AngleAxisf(M_PI, Eigen::Vector3f::UnitZ());

	camera cam(p, 30.0, 0.1, 1000.0, 800, 600);

	range_point_cloud_xyz pc(model(), cam);
	
	range_image ri = pc.to_range_image();
	ri.save_image("test.png");
	
	range_point_cloud_xyz pc2(ri, cam.get_projection());
	
	ply_writer<point_xyz> ply("revproj.ply");
	pc2.write(ply);


	
	
	int k = 100;
	int n = 0;
	float csz = optimal_grid_cell_length_for_knn(model(), k, 10.0);
	std::cout << "cell length: " << csz << std::endl;

	grid_point_cloud_full pc(model(), csz);
	for(auto& p : pc) p.color = 0xffffff;
	pc.find_nearest_neighbors(k, [&n](const auto&)->bool {
		return !(++n % 500);
	}, [k](const auto& p, const auto& nn) {
		std::cout << (void*)&p << " - " << omp_get_thread_num() << std::endl;

		int i = 0;
		rgb_color c(random_integer(256), random_integer(256), random_integer(256));
		for(const auto& p2 : nn) {
			if(++i == k) return;
			p2->color = c;
		}
	}, 0);
	
	ply_writer<point_full> ply("out.ply");
	pc.write(ply);*/
}

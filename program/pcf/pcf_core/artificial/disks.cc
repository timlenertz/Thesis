#include "disks.h"
#include "../util/random.h"
#include "../util/coordinates.h"
#include "../util/multi_dimensional_array.h"
#include "../geometry/math_constants.h"
#include <cmath>
#include <memory>
#include <utility>
#include <random>
#include <algorithm>
#include <iostream>


namespace pcf {


disks::disks(float width, unsigned number_of_disks, int seed) :
width_(width) {
	std::mt19937 gen;
	if(seed) gen.seed(seed);
	else gen = get_random_generator();
			
	for(auto i = 0; i < number_of_disks; ++i) {
		disk dsk;
		float hw = width / 2.0;
		
		dsk.o = Eigen::Vector3f(
			random_real<float>(-hw, +hw, gen),
			random_real<float>(-hw, +hw, gen),
			random_real<float>(0, width/5.0, gen)
		);
		
		float max_c = 6.0;
		dsk.n = Eigen::Vector3f(
			random_real<float>(-max_c, +max_c, gen),
			random_real<float>(-max_c, +max_c, gen),
			-1
		);
		dsk.n.normalize();
		
		dsk.r = random_real<float>(0, +hw, gen);
				
		disks_.push_back(dsk);
	}
}


camera_range_point_cloud_full<projection_image_camera> disks::make_projected_point_cloud(const projection_image_camera& cam) const {
	camera_range_point_cloud_full<projection_image_camera> rpc(cam);
	index_2dim ic;
	for(ic.y = 0; ic.y < cam.image_height() - 1; ++ic.y)
	for(ic.x = 0; ic.x < cam.image_width() - 1 ; ++ic.x) {
		rpc.at(ic).invalidate();

		auto ray = cam.ray(ic);

		float min_t = INFINITY;
		for(const disk& dsk : disks_) {
			Eigen::Vector3f o = dsk.o;
			Eigen::Hyperplane<float, 3> pl(dsk.n, dsk.o);
			float t = ray.intersection(pl);
			Eigen::Vector3f i = ray.intersectionPoint(pl);
			if((i - o).norm() > dsk.r) continue;
			
			if(t < min_t) {
				min_t = t;
				rpc.at(ic).revalidate();
				rpc.at(ic) = i;
				rpc.at(ic).set_normal(dsk.n);
			}
		}
	}
	return rpc;
}


unorganized_point_cloud_full disks::make_projected_point_cloud_without_occlusion(const projection_image_camera& cam) const {
	std::vector<point_full> pts;

	index_2dim ic;
	for(ic.y = 0; ic.y < cam.image_height() - 1; ++ic.y)
	for(ic.x = 0; ic.x < cam.image_width() - 1; ++ic.x) {
		auto ray = cam.ray(ic);

		float min_t = INFINITY;
		for(const disk& dsk : disks_) {
			Eigen::Vector3f o = dsk.o;
			Eigen::Hyperplane<float, 3> pl(dsk.n, dsk.o);
			float t = ray.intersection(pl);
			Eigen::Vector3f i = ray.intersectionPoint(pl);
			if((i - o).norm() > dsk.r) continue;
			
			point_full p(i);
			p.set_normal(dsk.n);
			pts.push_back(p);
		}
	}

	unorganized_point_cloud_full pc(pts.size());
	for(std::ptrdiff_t i = 0; i < pts.size(); ++i)
		pc[i] = pts[i];
	
	return pc;
}



unorganized_point_cloud_full disks::make_point_cloud(float density) const {
	std::vector<point_full> pts;
	
	for(const disk& dsk : disks_) {
		Eigen::Vector3f o = dsk.o;
		Eigen::Vector3f n = dsk.n;

		Eigen::Vector3f x(0, n[2], -n[1]);
		Eigen::Vector3f y = n.cross(x);
		x.normalize(); y.normalize();
		
		std::ptrdiff_t n_side = std::sqrt(density * pi) * dsk.r;

		float l_side = dsk.r / n_side;
		x = l_side * x; y = l_side * y;
				
		for(std::ptrdiff_t yi = -n_side; yi < n_side; ++yi)
		for(std::ptrdiff_t xi = -n_side; xi < n_side; ++xi) {
			Eigen::Vector3f dp = o + xi*x + yi*y;
			if((dp - o).norm() <= dsk.r) {
				pts.push_back(point_full(dp));
				pts.back().set_normal(n);
			}
		}
	}
	
	unorganized_point_cloud_full pc(pts.size());
	for(std::ptrdiff_t i = 0; i < pts.size(); ++i)
		pc[i] = pts[i];
	
	return pc;
}


}
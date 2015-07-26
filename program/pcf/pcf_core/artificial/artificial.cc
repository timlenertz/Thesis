#include "artificial.h"
#include "../util/random.h"
#include "../util/coordinates.h"
#include "../geometry/math_constants.h"
#include "../point_cloud/merge.h"
#include <Eigen/Eigen>
#include <cmath>
#include <memory>
#include <utility>
#include <random>
#include <iostream>
#include <cmath>

namespace pcf {


unorganized_point_cloud_full make_sphere_point_cloud(float radius, float density) {
	float sz = density * four_pi * radius * radius;
	unorganized_point_cloud_full pc(sz);
	for(point_full& p : pc) {
		float theta = random_real<float>(0, two_pi);
		float phi = std::acos( random_real<float>(-1, 1) );
		Eigen::Vector3f unit_pt(
			std::cos(theta) * std::sin(phi),
			std::sin(theta) * std::sin(phi),
			std::cos(phi)		
		);
		p = point_full(radius * unit_pt);
		p.set_normal(unit_pt);
	}
	return pc;
}


range_point_cloud_full make_plane_point_cloud(float side_length, float density) {
	std::size_t n_side = side_length * std::sqrt(density);
	range_point_cloud_full pc(n_side, n_side);
	
	for(std::ptrdiff_t yi = 0; yi < n_side; ++yi)
	for(std::ptrdiff_t xi = 0; xi < n_side; ++xi) {
		float x = (side_length * xi) / n_side;
		float y = (side_length * yi) / n_side;
		point_full p(x - side_length/2.0, y - side_length/2.0, 0);
		p.set_normal(Eigen::Vector3f(0, 0, 1));
		pc.at(xi, yi) = p;
	}
	
	return pc;
}


std::vector<point_full> make_plane_samples(std::size_t n, float side_length) {
	std::vector<point_full> pts;
	for(std::size_t i = 0; i != n; ++i) {
		float x = random_real<float>(0, side_length);
		float y = random_real<float>(0, side_length);
		point_full p(x - side_length/2.0, y - side_length/2.0, 0);
		p.set_normal(Eigen::Vector3f(0, 0, 1));
		pts.push_back(p);
	}
	return pts;
}


unorganized_point_cloud_full make_plane_point_cloud_random_distribution(float side_length, float density) {
	float sz = density * side_length * side_length;
	unorganized_point_cloud_full pc(sz);
	for(point_full& p : pc) {
		float x = random_real<float>(0, side_length);
		float y = random_real<float>(0, side_length);
		p = point_full(x - side_length/2.0, y - side_length/2.0, 0);
		p.set_normal(Eigen::Vector3f(0, 0, 1));
	}
	return pc;
}


camera_range_point_cloud_full<projection_image_camera> make_projected_plane_point_cloud(float side_length, const projection_image_camera& cam) {
	camera_range_point_cloud_full<projection_image_camera> rpc(cam);
	Eigen::Hyperplane<float, 3> pl(Eigen::Vector3f::UnitZ(), Eigen::Vector3f::Zero());
	index_2dim pc_ic, ic;
	for(ic.y = 0; ic.y < cam.image_height() - 1; ++ic.y)
	for(ic.x = 0; ic.x < cam.image_width() - 1 ; ++ic.x) {
		auto ray = cam.ray(ic);
		Eigen::Vector3f p = ray.intersectionPoint(pl);
		if(p[0] > -side_length/2.0 && p[0] < side_length/2.0 && p[1] > -side_length/2.0 && p[1] < side_length/2.0) {
			rpc.at(ic) = p;
			rpc.at(ic).set_normal(Eigen::Vector3f::UnitZ());
		} else {
			rpc.at(ic).invalidate();
		}
	}
	return rpc;
}


projection_image_camera parallel_projection_camera_at_angle(float radius, angle a, float elevation, std::size_t image_width) {
	pose ps;
	ps.position = Eigen::Vector3f(
		std::cos(a) * radius,
		std::sin(a) * radius,
		elevation
	);
	ps.orientation = Eigen::Quaternionf::FromTwoVectors(
		Eigen::Vector3f::UnitZ(),
		-ps.position
	).normalized();
		
	return projection_image_camera(
		ps,
		projection_bounding_box::symmetric_orthogonal(2*radius, 2*radius),
		image_width, image_width
	);
}


projection_image_camera parallel_projection_camera_top_down(float radius, float elevation, std::size_t image_width) {
	pose ps;
	ps.position = Eigen::Vector3f(0, 0, elevation);
	ps.look_at(Eigen::Vector3f::Zero());
	
	return projection_image_camera(
		ps,
		projection_bounding_box::symmetric_orthogonal(2*radius, 2*radius),
		image_width, image_width
	);
}


}
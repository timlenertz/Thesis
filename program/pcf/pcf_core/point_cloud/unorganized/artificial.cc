#include "artificial.h"
#include "../../util/random.h"
#include "../../geometry/math_constants.h"
#include "merge.h"
#include <Eigen/Eigen>
#include <cmath>
#include <memory>
#include <utility>

namespace pcf {

unorganized_point_cloud_xyz make_sphere_point_cloud(float radius, std::size_t sz) {
	unorganized_point_cloud_xyz pc(sz);
	for(point_xyz& p : pc) {
		float theta = random_real<float>(0, two_pi);
		float phi = std::acos( random_real<float>(-1, 1) );
		p = point_xyz(
			radius * std::cos(theta) * std::sin(phi),
			radius * std::sin(theta) * std::sin(phi),
			radius * std::cos(phi)		
		);
	}
	return pc;
}


unorganized_point_cloud_xyz make_sphere_point_cloud_with_density(float radius, float density) {
	float n = density * four_pi * radius * radius;
	return make_sphere_point_cloud(radius, n);
}


unorganized_point_cloud_xyz make_relief_point_cloud(float width, float density) {
	struct sphere {
		float radius;
		Eigen::Vector3f origin;
	};
	std::vector<sphere> spheres;
	
	std::vector<unorganized_point_cloud_xyz> clouds;
	
	auto point_in_any_sphere = [&](const Eigen::Vector3f& p, const sphere* except = nullptr) {
		for(const sphere& s : spheres) {
			if(&s == except) continue;
			if(distance_sq(p, s.origin) <= s.radius*s.radius) return true;
		}
		return false;
	};
		
	std::size_t number_of_spheres = 5;
	for(std::size_t i = 0; i < number_of_spheres; ++i) {
		sphere s;
		s.radius = random_real<float>(width/20.0, width/2.0);
		s.origin[0] = random_real<float>(-width/3.0, +width/3.0);
		s.origin[1] = random_real<float>(-width/3.0, +width/3.0);
		s.origin[2] = random_real<float>(-0.7*s.radius, -0.5*s.radius);
		spheres.push_back(s);
	}
	
	
	float hw = width / 2.0;
	for(const sphere& s : spheres) {
		auto pc = make_sphere_point_cloud_with_density(s.radius, density);
		pc.move(s.origin);
		pc.filter([&](const point_xyz& p) {
			Eigen::Vector3f wp = Eigen::Vector3f(p) + s.origin;
			return
				(wp[2] > 0.0) &&
				!point_in_any_sphere(wp, &s) &&
				(wp[0] > -hw) && (wp[0] < +hw) &&
				(wp[1] > -hw) && (wp[1] < +hw);
		});
		
		clouds.push_back(std::move(pc));
	}
	
	std::size_t number_of_plane_points = density * width * width;
	unorganized_point_cloud_xyz pc(number_of_plane_points);
	unorganized_point_cloud_xyz::iterator it = pc.begin();
	for(std::size_t i = 0; i < number_of_plane_points; ++i) {
		Eigen::Vector3f pt = Eigen::Vector3f::Zero();
		pt[0] = random_real<float>(-hw, +hw);
		pt[1] = random_real<float>(-hw, +hw);
		if(not point_in_any_sphere(pt)) *(it++) = point_xyz(pt);
	}
	clouds.push_back(std::move(pc));

	return merge_point_clouds(clouds.begin(), clouds.end());
}

}
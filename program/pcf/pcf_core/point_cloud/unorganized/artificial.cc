#include "artificial.h"
#include "../../util/random.h"
#include "../../geometry/math_constants.h"
#include "merge.h"
#include <Eigen/Eigen>
#include <cmath>
#include <memory>
#include <utility>
#include <random>

namespace pcf {

unorganized_point_cloud_full make_sphere_point_cloud(float radius, std::size_t sz) {
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


unorganized_point_cloud_full make_sphere_point_cloud_with_density(float radius, float density) {
	float n = density * four_pi * radius * radius;
	return make_sphere_point_cloud(radius, n);
}


relief::relief(float width, int seed) {
	std::mt19937 gen;
	if(seed) gen.seed(seed);
	else gen = get_random_generator();
			
	std::size_t number_of_engravings = 100;
	for(auto i = 0; i < number_of_engravings; ++i) {
		engraving e;
		e.x = random_real<float>(0.0, width, gen);
		e.y = random_real<float>(0.0, width, gen);	
		if(i < number_of_engravings / 4) {	
			e.radius = random_real<float>(0.2*width, 0.3*width, gen);
			e.depth = e.radius * random_real<float>(0.2, 0.4, gen);
		} else {
			e.radius = random_real<float>(0.02*width, 0.09*width, gen);
			e.depth = e.radius * random_real<float>(0.7, 2.2, gen);
		}
		e.out = (random_integer(0, 10, gen) < 3);
		engravings_.push_back(e);
	}
	
	width_ = width;
}


point_full relief::at(float x, float y) const {
	float z = 0.0;
	
	float der_x = 0, der_y = 0;
	
	for(const engraving& e : engravings_) {
		float dx = e.x - x;
		float dy = e.y - y;
		float d_sq = dx*dx + dy*dy;
		float r_sq = e.radius * e.radius;
		if(r_sq <= d_sq) continue;
		
		float dz = e.depth * std::sqrt(1.0 - d_sq/r_sq) * e.depth;
		if(e.out) dz = -dz;
		
		z += dz;
		
		float q = std::sqrt(r_sq) * std::sqrt(1.0 - (dx*dx + dy*dy)/r_sq);
		if(!e.out) q = -q;
		der_x -= e.depth * dx / q;
		der_y -= e.depth * dy / q;
	}
			
	point_full p(x - width_/2.0, y - width_/2.0, z);
	p.set_normal(-Eigen::Vector3f(der_x, der_y, -1));
	return p;
}


range_point_cloud_full relief::make_point_cloud(float density) const {
	std::size_t n_side = width_ * std::sqrt(density);
	
	range_point_cloud_full pc(n_side, n_side);
	
	for(std::ptrdiff_t yi = 0; yi < n_side; ++yi)
	for(std::ptrdiff_t xi = 0; xi < n_side; ++xi) {
		float x = (width_ * xi) / n_side;
		float y = (width_ * yi) / n_side;
		pc.at(xi, yi) = at(x, y);
	}
	
	return pc;
}

}
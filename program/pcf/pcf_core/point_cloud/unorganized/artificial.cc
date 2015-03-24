#include "artificial.h"
#include "../../util/random.h"
#include "../../geometry/math_constants.h"
#include <cmath>

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

}
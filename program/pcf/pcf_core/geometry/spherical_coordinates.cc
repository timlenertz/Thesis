#include "spherical_coordinates.h"
#include <ostream>
#include <cmath>
#include "../util/random.h"

namespace pcf {

spherical_coordinates spherical_coordinates::from_cartesian(const Eigen::Vector3f& c) {
	float r = c.norm();
	return spherical_coordinates(
		r,
		std::atan2(c[0], -c[2]),
		half_pi - std::acos(c[1] / r)
	);
}


Eigen::Vector3f spherical_coordinates::to_cartesian() const {
	return Eigen::Vector3f(
		std::cos(azimuth) * std::sin(elevation),
		std::cos(elevation),
		-std::sin(azimuth) * std::sin(elevation)
	) * radius;
}


bool spherical_coordinates::operator==(const spherical_coordinates& s) const {
	if(valid() && s.valid())
		return (radius == s.radius) && (azimuth == s.azimuth) && (elevation == s.elevation);
	else
		return (valid() == s.valid());
}


bool spherical_coordinates::operator!=(const spherical_coordinates& s) const {
	if(valid() && s.valid())
		return (radius != s.radius) || (azimuth != s.azimuth) || (elevation != s.elevation);
	else
		return (valid() != s.valid());
}


std::ostream& operator<<(std::ostream& str, const spherical_coordinates& c) {
	str << '(' << c.radius << ", " << c.azimuth << ", " << c.elevation << ')';
	return str;
}


spherical_coordinates spherical_coordinates::random_direction(float radius) {
	spherical_coordinates s;
	s.azimuth = random_real<float>(-pi, +pi);
	s.elevation = random_real<float>(-half_pi, +half_pi);
	s.radius = radius;
	return s;
}


}
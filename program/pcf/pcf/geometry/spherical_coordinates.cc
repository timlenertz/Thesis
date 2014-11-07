#include "spherical_coordinates.h"
#include <ostream>
#include <cmath>

namespace pcf {

spherical_coordinates spherical_coordinates::from_cartesian(const Eigen::Vector3f& c) {
	float r = c.norm();
	return spherical_coordinates(
		r,
		std::atan2(c[0], c[2]),
		std::asin(c[1] / r)
	);
}


Eigen::Vector3f spherical_coordinates::to_cartesian() const {
	return Eigen::Vector3f(
		std::cos(elevation) * std::sin(azimuth),
		std::sin(elevation),
		std::cos(elevation) * std::cos(azimuth)
	) * radius;
}


bool spherical_coordinates::operator==(const spherical_coordinates& s) const {
	return (radius == s.radius) && (azimuth == s.azimuth) && (elevation == s.elevation);
}


bool spherical_coordinates::operator!=(const spherical_coordinates& s) const {
	return (radius != s.radius) || (azimuth != s.azimuth) || (elevation != s.elevation);
}


std::ostream& operator<<(std::ostream& str, const spherical_coordinates& c) {
	str << '(' << c.radius << ", " << c.azimuth << ", " << c.elevation << ')';
	return str;
}


}
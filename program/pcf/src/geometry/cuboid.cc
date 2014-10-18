#include <cmath>
#include <ostream>

#include "cuboid.h"

namespace pcf {

cuboid cuboid::infinite() {
	const float inf = INFINITY;
	return cuboid(
		Eigen::Vector3f(+inf, +inf, +inf),
		Eigen::Vector3f(-inf, -inf, -inf)
	);
}

Eigen::Vector3f cuboid::center() const {
	return (origin + extremity) / 2;
}

Eigen::Vector3f cuboid::side_lengths() const {
	return (extremity - origin);
}

float cuboid::area() const {
	Eigen::Vector3f slen = side_lengths();
	return slen[0] * slen[1] * slen[2];
}

bool cuboid::contains(const Eigen::Vector3f& p) const {
	return
		(origin[0] <= p[0]) && (p[0] < extremity[0]) &&
		(origin[1] <= p[1]) && (p[1] < extremity[1]) &&
		(origin[2] <= p[2]) && (p[2] < extremity[2]);		
}


std::ostream& operator<<(std::ostream& str, const cuboid& c) {
	str << "[ (" << c.origin[0] << ", " << c.origin[1] << ", " << c.origin[2] << ");"
		   " (" << c.extremity[0] << ", " << c.extremity[1] << ", " << c.extremity[2] << ") [";
	return str;
}


}
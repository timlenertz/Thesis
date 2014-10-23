#include <cmath>
#include <cassert>
#include <ostream>

#include "cuboid.h"

namespace pcf {

cuboid::cuboid(const Eigen::Vector3f& o, const Eigen::Vector3f& e) :
origin(o), extremity(e) {
	for(std::ptrdiff_t i = 0; i < 3; ++i) assert(origin[i] <= extremity[i]);
}


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


bool cuboid::contains(const cuboid& cub) const {
	// TODO check < vs <=
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		if(cub.extremity[i] < origin[i] || cub.origin[i] > extremity[i]) return false;
	}
	return true;
}


std::array<Eigen::Vector3f, 8> cuboid::corners() const {
	return {
		Eigen::Vector3f(origin[0], origin[1], origin[2]),
		Eigen::Vector3f(origin[0], origin[1], extremity[2]),
		Eigen::Vector3f(origin[0], extremity[1], origin[2]),
		Eigen::Vector3f(origin[0], extremity[1], extremity[2]),
		Eigen::Vector3f(extremity[0], origin[1], origin[2]),
		Eigen::Vector3f(extremity[0], origin[1], extremity[2]),
		Eigen::Vector3f(extremity[0], extremity[1], origin[2]),
		Eigen::Vector3f(extremity[0], extremity[1], extremity[2])
	};
}


float cuboid::minimal_distance_sq(const cuboid& a, const cuboid& b) {
	float dist = 0;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		if(a.extremity[i] < b.origin[i]) {
			float d = b.origin[i] - a.extremity[i];
			dist += d * d;
		} else if(b.extremity[i] < a.origin[i]) {
			float d = a.origin[i] - b.extremity[i];
			dist += d * d;
		}
	}
	return dist;
}


float cuboid::maximal_distance_sq(const cuboid& a, const cuboid& b) {
	Eigen::Vector3f ap, bp;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		float d1 = std::abs(a.extremity[i] - b.origin[i]);
		float d2 = std::abs(b.extremity[i] - a.origin[i]);
		if(d1 > d2) {
			ap[i] = a.extremity[i];
			bp[i] = b.origin[i];
		} else {
			ap[i] = a.origin[i];
			bp[i] = b.extremity[i];		
		}
	}
	return (ap - bp).squaredNorm();
}


std::ostream& operator<<(std::ostream& str, const cuboid& c) {
	str << "[ (" << c.origin[0] << ", " << c.origin[1] << ", " << c.origin[2] << ");"
		   " (" << c.extremity[0] << ", " << c.extremity[1] << ", " << c.extremity[2] << ") [";
	return str;
}


}
#include <cmath>
#include <cassert>
#include <ostream>

#include "bounding_box.h"

namespace pcf {

bounding_box::bounding_box(const Eigen::Vector3f& o, const Eigen::Vector3f& e) :
origin(o), extremity(e) {
	for(std::ptrdiff_t i = 0; i < 3; ++i) assert(origin[i] <= extremity[i]);
}


Eigen::Vector3f bounding_box::center() const {
	return (origin + extremity) / 2;
}

Eigen::Vector3f bounding_box::side_lengths() const {
	return (extremity - origin);
}

float bounding_box::side_length(std::ptrdiff_t i) const {
	return (extremity[i] - origin[i]);
}


float bounding_box::volume() const {
	Eigen::Vector3f slen = side_lengths();
	return slen[0] * slen[1] * slen[2];
}

void bounding_box::expand_extremity(float ep) {
	extremity += Eigen::Vector3f(ep, ep, ep);
}

bool bounding_box::contains(const Eigen::Vector3f& p) const {
	return
		(origin[0] <= p[0]) && (p[0] < extremity[0]) &&
		(origin[1] <= p[1]) && (p[1] < extremity[1]) &&
		(origin[2] <= p[2]) && (p[2] < extremity[2]);		
}


bool bounding_box::contains(const bounding_box& cub) const {
	// TODO check < vs <=
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		if(cub.extremity[i] < origin[i] || cub.origin[i] > extremity[i]) return false;
	}
	return true;
}


std::array<Eigen::Vector3f, 8> bounding_box::corners() const {
	return {
		// Left cube
		Eigen::Vector3f(origin[0], origin[1], origin[2]),
		Eigen::Vector3f(origin[0], origin[1], extremity[2]),
		Eigen::Vector3f(origin[0], extremity[1], origin[2]),
		Eigen::Vector3f(origin[0], extremity[1], extremity[2]),
		
		// Right cube
		Eigen::Vector3f(extremity[0], origin[1], origin[2]),
		Eigen::Vector3f(extremity[0], origin[1], extremity[2]),
		Eigen::Vector3f(extremity[0], extremity[1], origin[2]),
		Eigen::Vector3f(extremity[0], extremity[1], extremity[2])
	};
}


std::array<bounding_box::edge, 12> bounding_box::edges() const {
	auto corn = corners();
	return {
		// Left cube
		edge(corn[0], corn[1]),
		edge(corn[1], corn[3]),
		edge(corn[3], corn[2]),
		edge(corn[2], corn[0]),
		
		// Right cube
		edge(corn[0+4], corn[1+4]),
		edge(corn[1+4], corn[3+4]),
		edge(corn[3+4], corn[2+4]),
		edge(corn[2+4], corn[0+4]),
		
		// Joining edges
		edge(corn[0], corn[0+4]),
		edge(corn[1], corn[1+4]),
		edge(corn[2], corn[2+4]),
		edge(corn[3], corn[3+4])
	};
}


float minimal_distance_sq(const bounding_box& a, const bounding_box& b) {
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


float maximal_distance_sq(const bounding_box& a, const bounding_box& b) {
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


float minimal_distance_sq(const Eigen::Vector3f& p, const bounding_box& b) {
	float dist = 0;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		if(p[i] < b.origin[i]) {
			float d = b.origin[i] - p[i];
			dist += d * d;
		} else if(p[i] > b.extremity[i]) {
			float d = p[i] - b.extremity[i];
			dist += d * d;
		}
	}
	return dist;
}


float maximal_distance_sq(const Eigen::Vector3f& p, const bounding_box& b) {
	float max_dist = 0;
	for(const Eigen::Vector3f& c : b.corners()) {
		float d = (p - c).squaredNorm();
		if(d > max_dist) max_dist = d;
	}
	return max_dist;
}



std::ostream& operator<<(std::ostream& str, const bounding_box& c) {
	str << "[ (" << c.origin[0] << ", " << c.origin[1] << ", " << c.origin[2] << ");"
		   " (" << c.extremity[0] << ", " << c.extremity[1] << ", " << c.extremity[2] << ") [";
	return str;
}


}
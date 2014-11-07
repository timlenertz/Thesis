#ifndef PCF_POINT_H_
#define PCF_POINT_H_

#include "point_xyz.h"
#include "point_full.h"

namespace pcf {

inline float euclidian_distance_sq(const point_xyz& a, const point_xyz& b) {
	return (b.homogeneous_coordinates - a.homogeneous_coordinates).squaredNorm();
}

inline float euclidian_distance(const point_xyz& a, const point_xyz& b) {
	return std::sqrt(euclidian_distance_sq(a, b));
}

}

#endif
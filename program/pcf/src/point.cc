#include "point.h"

namespace pcf {

float euclidian_distance_sq(const point_xyz& a, const point_xyz& b) {
	return (b.homogeneous_coordinates - a.homogeneous_coordinates).squaredNorm();
}

float euclidian_distance(const point_xyz& a, const point_xyz& b) {
	return std::sqrt(euclidian_distance_sq(a, b));
}

}
#include "point.h"

namespace pcf {

float distance_sq(const point_xyz& a, const point_xyz& b) {
	return (b.homogeneous_coordinates - a.homogeneous_coordinates).squaredNorm();
}

float distance(const point_xyz& a, const point_xyz& b) {
	return (b.homogeneous_coordinates - a.homogeneous_coordinates).norm();
}

}
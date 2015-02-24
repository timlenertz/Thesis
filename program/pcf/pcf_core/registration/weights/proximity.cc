#include "proximity.h"

namespace pcf {

float correspondences_proximity_weights::operator()(const point_xyz& a, const point_xyz& b) const {
	float d = distance(a, b);
	if(d < maximal_distance_) return 1.0f - d/maximal_distance_;
	else return 0.0f;
}

}
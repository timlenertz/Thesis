#ifndef PCF_CORRESPONDENCES_WEIGHTS_PROXIMITY_H_
#define PCF_CORRESPONDENCES_WEIGHTS_PROXIMITY_H_

#include "../../point.h"

namespace pcf {

/**
Assigns greater weight to point pairs that are closer.
When distance is greater than given maximal distance, assigns weight 0, i.e. the pair is rejected.
*/
class proximity_correspondences_weights {
private:
	const float maximal_distance_;

public:
	explicit proximity_correspondences_weights(float max_dist) :
		maximal_distance_(max_dist) { }

	float operator()(const point_xyz& a, const point_xyz& b) const {
		float d = distance(a, b);
		if(d < maximal_distance_) return 1.0f - d/maximal_distance_;
		else return 0.0f;
	}
};

}

#endif
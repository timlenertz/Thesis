#ifndef PCF_CORRESPONDENCES_WEIGHTS_PROXIMITY_H_
#define PCF_CORRESPONDENCES_WEIGHTS_PROXIMITY_H_

#include "../../point.h"

namespace pcf {

/**
Assigns greater weight to point pairs that are closer.
When distance is greater than given maximal distance, assigns weight 0, i.e. the pair is rejected.
*/
class correspondences_proximity_weights {
private:
	const float maximal_distance_;

public:
	explicit correspondences_proximity_weights(float max_dist) :
		maximal_distance_(max_dist) { }

	float operator()(const point_xyz&, const point_xyz&) const;
};

}

#endif
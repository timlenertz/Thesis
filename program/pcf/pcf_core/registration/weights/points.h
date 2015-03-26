#ifndef PCF_CORRESPONDENCES_WEIGHTS_POINTS_H_
#define PCF_CORRESPONDENCES_WEIGHTS_POINTS_H_

#include "../../point.h"

namespace pcf {

/**
Assigns weight according to weights of the two points.
Ratio indicates to what level each point's weight is taken. 0.0 means take first point's weight,
1.0 second point's weight, 0.5 mean of both.
*/
class points_correspondences_weights {
private:
	const float ratio1_;
	const float ratio2_;

public:
	explicit points_correspondences_weights(float ratio = 0.5) :
		ratio1_(1.0f - ratio), ratio2_(ratio) { }

	template<typename Point1, typename Point2>
	float operator()(const Point1& p1, const Point2& p2) const {
		float w1 = p1.get_weight();
		float w2 = p2.get_weight();
		return ratio1_*w1 + ratio2_*w2;
	}
};

}

#endif
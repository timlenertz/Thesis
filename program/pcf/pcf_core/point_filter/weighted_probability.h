#ifndef PCF_WEIGHTED_PROBABILITY_POINT_FILTER_H_
#define PCF_WEIGHTED_PROBABILITY_POINT_FILTER_H_

#include "point_filter.h"
#include "../util/random.h"

namespace pcf {

/**
Randomly accept points with given probability, weighted by point's weight.
Points with weight 2 are twice as likely to be chosen. If point weights are normalized (i.e. they sum up to number of points), than the probability will still be approximately equal to the number of point chosen. Takes xyz points to have weight 1.
*/
class weighted_probability_point_filter : public point_filter {
private:
	const float probability_;

public:
	explicit weighted_probability_point_filter(float prob) :
		probability_(prob) { }

	bool operator()(const point_xyz&) {
		float r = random_real(0.0f, 1.0f);
		return (r < probability_);
	}
	
	bool operator()(const point_full& p) {
		float r = random_real(0.0f, 1.0f);
		return (r < probability_ * p.get_weight());
	}
};

}

#endif
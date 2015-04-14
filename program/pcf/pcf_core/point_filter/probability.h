#ifndef PCF_PROBABILITY_POINT_FILTER_H_
#define PCF_PROBABILITY_POINT_FILTER_H_

#include "point_filter.h"
#include "../util/random.h"

namespace pcf {

/**
Randomly accept points with given probability.
*/
class probability_point_filter : public point_filter {
private:
	const float probability_;

public:
	explicit probability_point_filter(float prob) :
		probability_(prob) { }

	bool operator()(const point_xyz&) const {
		float r = random_real(0.0f, 1.0f);
		return (r < probability_);
	}
};

}

#endif
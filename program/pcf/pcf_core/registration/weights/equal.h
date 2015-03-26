#ifndef PCF_CORRESPONDENCES_WEIGHTS_EQUAL_H_
#define PCF_CORRESPONDENCES_WEIGHTS_EQUAL_H_

#include "../../point.h"

namespace pcf {

/**
Assigns equal weight to all point correspondence pairs.
Always assigns weight 1.
*/
class equal_correspondences_weights {
public:
	float operator()(const point_xyz&, const point_xyz&) const { return 1.0; }
};

}

#endif
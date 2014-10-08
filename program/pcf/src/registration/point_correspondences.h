#ifndef PCF_POINT_CORRESPONDENCES_H_
#define PCF_POINT_CORRESPONDENCES_H_

#include <vector>
#include "../point.h"

namespace pcf {

class point_correspondences {	
public:
	struct correspondence {
		const point_xyz& source;
		const point_xyz& target;
		float weight = 1.0;
	};

public:
	std::vector<correspondence> correspondences_;
};

}

#endif
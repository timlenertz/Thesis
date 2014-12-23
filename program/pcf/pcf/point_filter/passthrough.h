#ifndef PCF_PASSTHROUGH_POINT_FILTER_H_
#define PCF_PASSTHROUGH_POINT_FILTER_H_

#include "point_filter.h"

namespace pcf {

class passthrough_point_filter : public point_filter {
public:
	bool operator()(const point_xyz&) {
		return true;
	}
};

}

#endif
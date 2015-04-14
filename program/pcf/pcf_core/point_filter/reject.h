#ifndef PCF_REJECT_POINT_FILTER_H_
#define PCF_REJECT_POINT_FILTER_H_

#include "point_filter.h"

namespace pcf {

/**
Reject every point.
*/
class reject_point_filter : public point_filter {
public:
	bool operator()(const point_xyz&) const {
		return false;
	}
};

}

#endif
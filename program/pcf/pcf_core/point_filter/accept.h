#ifndef PCF_ACCEPT_POINT_FILTER_H_
#define PCF_ACCEPT_POINT_FILTER_H_

#include "point_filter.h"

namespace pcf {

/**
Accept any point.
*/
class accept_point_filter : public point_filter {
public:
	bool operator()(const point_xyz&) {
		return true;
	}
};

}

#endif
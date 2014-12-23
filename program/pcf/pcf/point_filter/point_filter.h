#ifndef PCF_POINT_FILTER_H_
#define PCF_POINT_FILTER_H_

#include "../point.h"
#include <type_traits>

namespace pcf {

class point_filter {
protected:
	point_filter() = default;

public:
	using is_parallelizable = std::true_type;

	void reset() { }
};

}

#endif
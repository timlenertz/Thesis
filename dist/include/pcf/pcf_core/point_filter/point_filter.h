#ifndef PCF_POINT_FILTER_H_
#define PCF_POINT_FILTER_H_

#include "../point.h"
#include <type_traits>

namespace pcf {

/**
Base class for point filter.
Point filters accept or reject a point. Can be combined by conjunction and disjunction. Uses static polymorphism (templates), since it is evaluated per point.
*/
class point_filter {
protected:
	point_filter() = default;

public:
	using is_parallelizable = std::true_type;

	void reset() { }
};

}

#endif
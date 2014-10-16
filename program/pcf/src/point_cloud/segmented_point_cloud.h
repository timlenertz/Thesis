#ifndef PCF_SEGMENTED_POINT_CLOUD_H_
#define PCF_SEGMENTED_POINT_CLOUD_H_

#include <vector>
#include "point_cloud.h"

namespace pcf {

/**
Point cloud where subsets of points are grouped together.
*/
template<typename Point, typename Allocator = std::allocator<Point>>
class segmented_point_cloud : public point_cloud<Point, Allocator> {
	using super = point_cloud<Point, Allocator>;

protected:
	struct segment {
		const Point* start;
		std::size_t size;
	};
	
	std::vector<segment> segments_;

	template<typename Condition_func>
	segment& make_segment(const Point* start, Condition_func cond);
};

}

#include "segmented_point_cloud.tcc"

#endif
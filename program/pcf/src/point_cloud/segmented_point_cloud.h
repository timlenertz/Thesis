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
		Point* start;
		std::size_t size;
	};
	
	segment full_segment_() const {
		return { super::buffer_, super::size() };
	}
	
	template<typename Condition_func>
	segment make_segment(Condition_func cond, Point* start, const Point* end = nullptr);
	
	template<typename Other_point, typename Distance_func>
	const Point& find_closest_point_in_segment_(const Other_point& from, const segment& seg, Distance_func dist) const;
	
	using super::super;
};

}

#include "segmented_point_cloud.tcc"

#endif
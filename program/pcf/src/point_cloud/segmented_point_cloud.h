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
	class segment;
	
	segment full_segment_() {
		return segment(super::buffer_, super::buffer_end_);
	}
	
	template<typename Condition_func>
	segment make_segment(Condition_func cond, Point* start, Point* limit = nullptr);
	
	template<typename Other_point, typename Distance_func>
	const Point& find_closest_point_in_segment_(const Other_point& from, const segment& seg, Distance_func dist) const;
	
	using super::super;
};



template<typename Point, typename Allocator>
class segmented_point_cloud<Point, Allocator>::segment {
protected:
	Point* start_;
	Point* end_;

public:
	segment() = delete;
	segment(Point* s, Point* e) : start_(s), end_(e) { }
	
	Point* start() { return start_; }
	Point* end() { return end_; }
	const Point* start() const { return start_; }
	const Point* end() const { return end_; }
	
	std::size_t size() const { return end_ - start_; }
};

}

#include "segmented_point_cloud.tcc"

#endif
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
	segment make_segment_(Condition_func cond, Point* begin, Point* limit);
	
	template<typename Index_func>
	std::vector<segment> make_segments_(Index_func cond, std::size_t segment_count, Point* begin, Point* limit);
		
	template<typename Condition_func>
	bool verify_segment_(Condition_func cond, const segment&) const;
		
	using super::super;
};



template<typename Point, typename Allocator>
class segmented_point_cloud<Point, Allocator>::segment {
protected:
	Point* begin_;
	Point* end_;

public:
	using iterator = Point*;
	using const_iterator = const Point*;

	segment() = default;
	segment(Point* s, Point* e) : begin_(s), end_(e) { }
	
	iterator begin() { return begin_; }
	const const_iterator begin() const { return begin_; }
	const const_iterator cbegin() const { return begin_; }

	iterator end() { return end_; }
	const_iterator end() const { return end_; }
	const_iterator cend() const { return end_; }
	
	std::size_t size() const { return end_ - begin_; }
	bool empty() const { return end_ == begin_; }
};

}

#include "segmented_point_cloud.tcc"

#endif
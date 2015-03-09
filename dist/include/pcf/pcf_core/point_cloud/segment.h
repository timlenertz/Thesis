#ifndef PCF_POINT_CLOUD_SEGMENT_H_
#define PCF_POINT_CLOUD_SEGMENT_H_

#include <utility>
#include <vector>
#include <Eigen/Eigen>
#include "../geometry/bounding_box.h"
#include "../point.h"

namespace pcf {

/**
Segment of a point cloud.
Interval of points in a point cloud. Does not own the memory. Does not differenciate between const and non-const access.
However, Point may be a const type. Point clouds provide segment and const_segment typedefs. (I.e. same principle as iterators)
*/
template<typename Point>
class point_cloud_segment {
protected:
	Point* begin_;
	Point* end_;
			
public:
	using point_type = Point;
	using iterator = Point*;

	point_cloud_segment() = default;
	point_cloud_segment(Point* s, Point* e) :
		begin_(s), end_(e) { }
	template<typename Other_point> point_cloud_segment(const point_cloud_segment<Other_point>& seg) :
		begin_(seg.begin()), end_(seg.end()) { }
	
	point_cloud_segment& operator=(const point_cloud_segment&) = default;
	
	bool in_bounds(std::ptrdiff_t i) const { return i >= 0 && i < size(); }
	
	Point& at(std::ptrdiff_t i) { assert(in_bounds(i)); return begin_[i]; }
	Point& operator[](std::ptrdiff_t i) { return at(i); }
	
	bool operator==(const point_cloud_segment& seg) const {
		return (begin_ == seg.begin_) && (end_ == seg.end_);
	}
	
	bool operator!=(const point_cloud_segment& seg) const {
		return (begin_ != seg.begin_) || (end_ != seg.end_);
	}
	
	bool operator<(const point_cloud_segment& seg) const { return (begin_ < seg.begin_); }
	bool operator<=(const point_cloud_segment& seg) const { return (begin_ <= seg.begin_); }
	bool operator>(const point_cloud_segment& seg) const { return (begin_ > seg.begin_); }
	bool operator>=(const point_cloud_segment& seg) const { return (begin_ >= seg.begin_); }
	
	bool contains(const Point&) const;
	bool contains(const point_cloud_segment&) const;
	bool intersects(const point_cloud_segment&) const;
	
	Point* data() const { return begin_; }
	Point* data_end() const { return end_; }
	
	iterator begin() const { return begin_; }
	iterator end() const { return end_; }
	std::size_t size() const { return end_ - begin_; }
	bool empty() const { return end_ == begin_; }
		
	bounding_box box(float ep = 0.0) const;
	Eigen::Vector3f center_of_mass() const;
	
	template<typename Index_func, typename Output_iterator>
	void partition_into_segments(Index_func idx, std::size_t n, Output_iterator it) const;
};

extern template class point_cloud_segment<point_xyz>;
extern template class point_cloud_segment<point_full>;

}

#include "segment.tcc"

#endif
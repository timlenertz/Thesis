#ifndef PCF_POINT_CLOUD_SEGMENT_H_
#define PCF_POINT_CLOUD_SEGMENT_H_

#include <utility>
#include <vector>
#include <Eigen/Eigen>
#include "../geometry/bounding_box.h"

namespace pcf {

/**
Segment of a point cloud.
Interval of points in a point cloud. Does not own the memory. Const access allows only for reading the const Points.
Non-const access allows non-const access to Points, and reordering operations.
*/
template<typename Point>
class point_cloud_segment {
protected:
	Point* begin_;
	Point* end_;
			
public:
	using point_type = Point;
	using iterator = Point*;
	using const_iterator = const Point*;

	point_cloud_segment() = default;
	point_cloud_segment(const point_cloud_segment&) = default;
	point_cloud_segment(Point* s, Point* e) : begin_(s), end_(e) { }
	
	bool in_bounds(std::ptrdiff_t i) const { return i >= 0 && i < size(); }
	Point& operator[](std::ptrdiff_t i) { assert(in_bounds(i)); return begin_[i]; }
	const Point& operator[](std::ptrdiff_t i) const { assert(in_bounds(i)); return begin_[i]; }
	
	Point* data() { return begin_; }
	const Point* data() const { return begin_; }
	const Point* data_end() const { return end_; }
	
	iterator begin() { return begin_; }
	const const_iterator begin() const { return begin_; }
	const const_iterator cbegin() const { return begin_; }

	iterator end() { return end_; }
	const_iterator end() const { return end_; }
	const_iterator cend() const { return end_; }
	
	std::size_t size() const { return end_ - begin_; }
	bool empty() const { return end_ == begin_; }
		
	bounding_box box(float ep = 0.0) const;
	Eigen::Vector3f center_of_mass() const;
	
	template<typename Index_func, typename Output_iterator>
	void partition_into_segments(Index_func idx, std::size_t n, Output_iterator it);
};

}

#include "point_cloud_segment.tcc"

#endif
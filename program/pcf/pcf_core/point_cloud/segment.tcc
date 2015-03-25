#include <algorithm>
#include <cassert>
#include <iostream>
#include "../point_algorithm.h"

namespace pcf {

template<typename Point>
bool point_cloud_segment<Point>::contains(const Point& p) const {
	return (&p >= begin_) && (&p < end_);
}


template<typename Point>
bool point_cloud_segment<Point>::contains(const point_cloud_segment& seg) const {
	return contains(*seg.begin_) && contains(*(seg.end_ - 1));
}


template<typename Point>
bool point_cloud_segment<Point>::intersects(const point_cloud_segment& seg) const {
	return contains(*seg.begin_) || contains(*(seg.end_ - 1));
}


template<typename Point>
bounding_box point_cloud_segment<Point>::box(float ep) const {
	return compute_bounding_box(begin_, end_);
}


template<typename Point>
Eigen::Vector3f point_cloud_segment<Point>::center_of_mass() const {
	std::size_t n = 0;
	Eigen::Vector4f sum = Eigen::Vector4f::Zero();
	for(auto&& p : *this) {
		if(! p.valid()) continue;
		sum += p.homogeneous_coordinates;
		++n;
	}
	return (sum / n).head(3);
}



template<typename Point> template<typename Index_func, typename Output_iterator>
void point_cloud_segment<Point>::partition_into_segments(Index_func idx, std::size_t n, Output_iterator it) const {
	point_cloud_segment cur = *this;
	Point* split = nullptr;
	for(std::size_t i = 0; i < n; ++i) {
		auto cond = [i, &idx](const Point& p) { return (idx(p) == i); };
		split = std::partition(cur.begin(), cur.end(), cond);
		*(it++) = point_cloud_segment(cur.begin(), split);
		cur.begin_ = split;
	}
	assert(split == end());
}


}
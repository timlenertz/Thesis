#include <stdexcept>
#include <algorithm>
#include <cassert>

namespace pcf {

template<typename Point, typename Allocator> template<typename Condition_func>
auto segmented_point_cloud<Point, Allocator>::make_segment(Condition_func cond, Point* start, Point* end) -> segment {
	if(start == super::buffer_end_) return segment(start, start);
	
	else if(start < super::buffer_ || start > super::buffer_end_)
		throw std::invalid_argument("Segment start not within point cloud buffer.");

	if(! end) end = super::buffer_end_;
	else if(end < start || end > super::buffer_end_ ) throw std::invalid_argument("Segment end invalid.");
	
	Point* np = start;
	for(Point* p = start; p < end; ++p)
		if(p->valid() && cond(*p)) std::swap(*(np++), *p);

	return segment(start, np);
}


template<typename Point, typename Allocator> template<typename Other_point, typename Distance_func>
const Point& segmented_point_cloud<Point, Allocator>::find_closest_point_in_segment_(const Other_point& from, const segment& seg, Distance_func dist) const {
	return super::find_closest_point(from, dist, seg.start(), seg.end());
}


}
#include <stdexcept>
#include <algorithm>
#include <cassert>

namespace pcf {

template<typename Point, typename Allocator> template<typename Condition_func>
auto segmented_point_cloud<Point, Allocator>::make_segment(Condition_func cond, Point* start, const Point* end) -> segment {
	if(start == super::buffer_end_) return { super::buffer_end_, 0 };
	else if(start < super::buffer_ || start > super::buffer_end_)
		throw std::invalid_argument("Segment start not within point cloud buffer.");

	if(! end) end = super::buffer_end_;
	else if(end < start || end > super::buffer_end_ ) throw std::invalid_argument("Segment end invalid.");
	
	Point* np = start;
	for(Point* p = start; p < end; ++p)
		if(p->valid() && cond(*p)) std::swap(*(np++), *p);

	return { start, std::ptrdiff_t(np - start) };
}


template<typename Point, typename Allocator> template<typename Other_point, typename Distance_func>
const Point& segmented_point_cloud<Point, Allocator>::find_closest_point_in_segment_(const Other_point& from, const segment& seg, Distance_func dist) const {
	float minimal_distance = INFINITY;
	Point* closest_point = nullptr;

	const Point* end = seg.start + seg.size;
	for(Point* p = seg.start; p < end; ++p) {
		if(!super::all_valid_) if(! p->valid()) continue;
		float d = dist(*p, from);								
		if(d < minimal_distance) {
			minimal_distance = d;
			closest_point = p;
		}
	}
	
	return *closest_point;
}


}
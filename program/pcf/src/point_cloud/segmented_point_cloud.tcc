#include <stsexcept>
#include <cassert>

namespace pcf {

template<typename Point, typename Allocator> template<typename Condition_func>
auto segmented_point_cloud<Point, Allocator>::make_segment(const Point* start, Condition_func cond) -> segment& {
	assert(! super::fixed_order_);
	if(start < buffer_ || start >= buffer_end_) throw std::invalid_argument("Segment start not within point cloud buffer.");
	
	Point* np = start;
	for(Point* p = start; p < super::buffer_end_; ++p) if(p->valid() && cond(*p)) {
		*(np++) = *p;
	}
	
	segment seg;
	seg.start = start;
	seg.size = np - start;
	
	segments_.push_back(seg);
	return segments_.back();
}

}
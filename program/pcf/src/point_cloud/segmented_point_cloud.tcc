#include <stdexcept>
#include <algorithm>
#include <functional>
#include <cassert>

namespace pcf {

template<typename Point, typename Allocator> template<typename Condition_func>
auto segmented_point_cloud<Point, Allocator>::make_segment_(Condition_func cond, Point* begin, Point* limit) -> segment {
	if(begin == limit) return segment(begin, begin);
	else if(begin < super::buffer_ || begin > super::buffer_end_) throw std::invalid_argument("Segment start not within point cloud buffer.");
	else if(limit < begin || limit > super::buffer_end_ ) throw std::invalid_argument("Segment end invalid.");
	
	Point* np = begin;
	for(Point* p = begin; p < limit; ++p)
		if(p->valid() && cond(*p)) std::swap(*(np++), *p);

	auto i = std::rand();
	for(Point* p = begin; p < np; ++p) mark_point(*p, i);


	return segment(begin, np);
}


template<typename Point, typename Allocator> template<typename Index_func>
auto segmented_point_cloud<Point, Allocator>::make_segments_(Index_func cond, std::size_t segment_count, Point* begin, Point* limit) -> std::vector<segment> {
	// TODO parallelize
	std::vector<segment> segs;
	segs.reserve(segment_count);
	for(std::ptrdiff_t i = 0; i < segment_count; ++i) {
		auto seg_cond = [&cond, &i](const Point& p) {
			return (cond(p) == i);
		};
		segment seg = make_segment_(seg_cond, begin, limit);
		begin = seg.end();
		
		segs.push_back(seg);
	}
		
	return segs;
}


template<typename Point, typename Allocator>
template<typename Condition_func>
bool segmented_point_cloud<Point, Allocator>::verify_segment_(Condition_func cond, const segment& seg) const {
	for(const Point& p : seg) if(! cond(p)) {
		std::cout << "c=" << cond(p) << p << std::endl;
		return false;
	}
	return true;
}


}
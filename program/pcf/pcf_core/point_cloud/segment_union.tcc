#include "segment_union_iterator.h"
#include <utility>
#include <algorithm>

namespace pcf {

template<typename Point>
void point_cloud_segment_union<Point>::initialize_() {
	// Remove empty segments
	segments_.erase( std::remove_if(
		segments_.begin(),
		segments_.end(),
		[](const segment_type& seg) { return seg.size() == 0; }
	), segments_.end() );
	if(segments_.size() == 0) return;
	
	// Sort by start address
	std::sort(segments_.begin(), segments_.end());
	
	// Remove intersections and join neighboring segments
	bool repeat = true;
	while(repeat && segments_.size() > 1) {	
		repeat = false;
		
		auto prev = segments_.begin();
		auto seg = prev; ++seg;
		while(seg != segments_.end()) {
			if(prev->contains(*seg)) {
				// Previous segment contains current one; erase current
				seg = segments_.erase(seg);
				repeat = true;
			} else if(seg->begin() <= prev->end()){
				// Segments directly follow one another, or previous contains head of current; join
				*prev = segment_type(prev->begin(), seg->end());
				seg = segments_.erase(seg);
				repeat = true;
			} else {
				prev = seg;
				++seg;
			}
		}
	}
}


template<typename Point>
point_cloud_segment_union<Point>::point_cloud_segment_union(std::initializer_list<segment_type> segs) :
segments_(segs) {
	initialize_();
}


template<typename Point> template<typename Container>
point_cloud_segment_union<Point>::point_cloud_segment_union(Container&& segs) :
segments_(std::forward<Container>(segs)) {
	initialize_();
}


template<typename Point>
std::size_t point_cloud_segment_union<Point>::size() const {
	std::size_t sz = 0;
	for(auto&& seg : segments_) sz += seg.size();
	return sz;
}



template<typename Point>
auto point_cloud_segment_union<Point>::begin() -> iterator {
	if(! segments_.empty()) return iterator(segments_.begin(), segments_.end());
	else return iterator(nullptr);
}


template<typename Point>
auto point_cloud_segment_union<Point>::end() -> iterator {
	if(! segments_.empty()) return iterator(segments_.back().end());
	else return iterator(nullptr);
}

}
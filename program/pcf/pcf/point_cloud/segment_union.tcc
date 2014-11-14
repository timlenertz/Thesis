#include "segment_union_iterator.h"
#include <utility>
#include <algorithm>

namespace pcf {

template<typename Point>
void point_cloud_segment_union<Point>::initialize_() {
	// The segments are ordered by their beginning address due to std::set
	// Join or erase segments as needed...
	std::sort(segments_.begin(), segments_.end());
	bool repeat = true;
	while(repeat) {
		typename segments_set::iterator prev = segments_.begin();
		typename segments_set::iterator seg = prev;
		for(++seg; seg != segments_.end();) {
			if(prev->contains(*seg)) {
				// Previous segment contains current one; erase current
				segments_.erase(seg);
				repeat = true;
			} else if(seg->begin() <= prev->end()){
				// Segments directly follow one another, or previous contains head of current; join
				*prev = segment_type(prev->begin(), seg->end());
				segments_.erase(seg);
				repeat = true;
			} else if(seg->begin() == seg->end()) {
				// Empty segment; erase
				segments_.erase(seg);
				repeat = true;
			} else {
				++seg;
			}
			prev = seg;
		}
	}
}

template<typename Point>
point_cloud_segment_union<Point>::point_cloud_segment_union(std::initializer_list<segment_type> segs) :
segments_(segs) {
	initialize_();
}


template<typename Point>
point_cloud_segment_union<Point>::point_cloud_segment_union(const std::vector<segment_type>& segs) :
segments_(segs) {
	initialize_();
}

template<typename Point>
point_cloud_segment_union<Point>::point_cloud_segment_union(std::vector<segment_type>&& segs) :
segments_(std::move(segs)) {
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
	return iterator(segments_.begin());
}


template<typename Point>
auto point_cloud_segment_union<Point>::end() -> iterator {
	return iterator(segments_.back().end());
}


template<typename Point>
auto point_cloud_segment_union<Point>::cbegin() const -> const_iterator {
	return const_iterator(segments_.cbegin());
}


template<typename Point>
auto point_cloud_segment_union<Point>::cend() const -> const_iterator {
	return const_iterator(segments_.back().cend());
}


}
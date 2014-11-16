#ifndef PCF_POINT_CLOUD_SEGMENT_UNION_ITERATOR_H_
#define PCF_POINT_CLOUD_SEGMENT_UNION_ITERATOR_H_

#include <iostream>
#include "segment_union.h"

namespace pcf {

template<typename Point>
class point_cloud_segment_union<Point>::iterator :
public std::iterator<std::forward_iterator_tag, Point> {
private:
	using segments_set_iterator = typename segments_set::iterator;
	using segment_iterator = typename segment_type::iterator;
	
	segments_set_iterator segment_current_;
	segments_set_iterator segment_end_;
	segment_iterator current_;
	segment_iterator end_;
	
	void load_segment_() {
		if(segment_current_ == segment_end_) return;
		current_ = segment_current_->begin();
		end_ = segment_current_->end();
	}
			
public:
	iterator(segments_set_iterator seg, segments_set_iterator segend) :
		segment_current_(seg), segment_end_(segend) { load_segment_(); }
		
	explicit iterator(segment_iterator ed) : current_(ed) { }
		
	iterator(const iterator&) = default;
	
	Point& operator*() const { return *current_; }
	Point* operator->() const { return & operator*(); }
	
	bool operator==(const iterator& it) const { return (current_ == it.current_); }
	bool operator!=(const iterator& it) const { return (current_ != it.current_); }
	
	iterator& operator++() {
		++current_;
		if(current_ == end_) {
			++segment_current_;
			load_segment_();
		}
		return *this;
	}
	
	iterator operator++(int) {
		iterator old = *this;
		operator++();
		return old;
	}
};

}

#endif
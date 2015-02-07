#ifndef PCF_POINT_CLOUD_SEGMENT_UNION_ITERATOR_H_
#define PCF_POINT_CLOUD_SEGMENT_UNION_ITERATOR_H_

#include <iostream>
#include "segment_union.h"
#include "../point.h"

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
	
	void load_segment_();
			
public:
	iterator(segments_set_iterator seg, segments_set_iterator segend);
	explicit iterator(segment_iterator ed);
		
	iterator(const iterator&) = default;
	
	Point& operator*() const { return *current_; }
	Point* operator->() const { return & operator*(); }
	
	bool operator==(const iterator& it) const { return (current_ == it.current_); }
	bool operator!=(const iterator& it) const { return (current_ != it.current_); }
	
	iterator& operator++();
	iterator operator++(int);
};

extern template class point_cloud_segment_union<point_xyz>;
extern template class point_cloud_segment_union<point_full>;

}

#include "segment_union_iterator.tcc"

#endif
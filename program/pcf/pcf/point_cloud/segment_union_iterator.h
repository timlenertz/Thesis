#ifndef PCF_POINT_CLOUD_SEGMENT_UNION_ITERATOR_H_
#define PCF_POINT_CLOUD_SEGMENT_UNION_ITERATOR_H_

#include "segment_union.h"

namespace pcf {

template<typename Point>
class point_cloud_segment_union<Point>::iterator :
public std::iterator<std::forward_iterator_tag, Point> {
private:
	using segments_set_iterator = typename segments_set::iterator;
	using segment_iterator = typename segment_type::iterator;
	
	segments_set_iterator segment_;
	segment_iterator current_;
	segment_iterator end_;
	
public:
	explicit iterator(segments_set_iterator seg) :
		segment_(seg), end_(seg->end()), current_(seg->begin()) { }
	
	explicit iterator(segment_iterator end) :
		current_(end), end_(end) { } ///< Create end iterator.
		
	iterator(const iterator&) = default;
	
	Point& operator*() const { return *current_; }
	Point* operator->() const { return &(*current_); }
	
	bool operator==(const iterator& it) const { return (current_ == it.current_); }
	bool operator!=(const iterator& it) const { return (current_ != it.current_); }
	bool operator<(const iterator& it) const { return (current_ < it.current_); }
	bool operator<=(const iterator& it) const { return (current_ <= it.current_); }
	bool operator>(const iterator& it) const { return (current_ > it.current_); }
	bool operator>=(const iterator& it) const { return (current_ >= it.current_); }
	
	iterator& operator++() {
		++current_;
		if(current_ == end_) {
			++segment_;
			current_ = segment_->begin();
			end_ = segment_->end();
		}
		return *this;
	}
	
	iterator operator++(int) {
		iterator old = *this;
		operator++();
		return old;
	}
};



template<typename Point>
class point_cloud_segment_union<Point>::const_iterator :
public std::iterator<std::forward_iterator_tag, const Point> {
private:
	using segments_set_iterator = typename segments_set::const_iterator;
	using segment_iterator = typename segment_type::const_iterator;
	
	segments_set_iterator segment_;
	segment_iterator current_;
	segment_iterator end_;
	
public:
	explicit const_iterator(segments_set_iterator seg) :
		segment_(seg), end_(seg->end()), current_(seg->begin()) { }
	
	explicit const_iterator(segment_iterator end) :
		current_(end), end_(end) { } ///< Create end iterator.
		
	const_iterator(const const_iterator&) = default;
	
	Point& operator*() const { return *current_; }
	Point* operator->() const { return &(*current_); }
	
	bool operator==(const iterator& it) const { return (current_ == it.current_); }
	bool operator!=(const iterator& it) const { return (current_ != it.current_); }
	
	const_iterator& operator++() {
		++current_;
		if(current_ == end_) {
			++segment_;
			current_ = segment_->begin();
			end_ = segment_->end();
		}
		return *this;
	}
	
	const_iterator operator++(int) {
		const_iterator old = *this;
		operator++();
		return old;
	}
};




}

#endif
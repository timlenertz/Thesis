namespace pcf {

template<typename Point>
void point_cloud_segment_union<Point>::iterator::load_segment_() {
	if(segment_current_ == segment_end_) return;
	current_ = segment_current_->begin();
	end_ = segment_current_->end();
}
		

template<typename Point>
point_cloud_segment_union<Point>::iterator::iterator(segments_set_iterator seg, segments_set_iterator segend) :
	segment_current_(seg), segment_end_(segend) { load_segment_(); }
	
	
template<typename Point>
point_cloud_segment_union<Point>::iterator::iterator(segment_iterator ed) : current_(ed) { }


template<typename Point>
auto point_cloud_segment_union<Point>::iterator::operator++() -> iterator& {
	++current_;
	if(current_ == end_) {
		++segment_current_;
		load_segment_();
	}
	return *this;
}


template<typename Point>
auto point_cloud_segment_union<Point>::iterator::operator++(int) -> iterator {
	iterator old = *this;
	operator++();
	return old;
}


}
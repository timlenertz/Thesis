#include <cmath>

namespace pcf {

template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(range_point_cloud_importer& imp, const Allocator& alloc) :
super(imp.size(), false, alloc),
image_({imp.columns(), imp.rows()}, super::begin_) {
	super::resize_(super::capacity());
	for(Point* buf = super::begin_; buf < super::end_; buf += width())
		imp.read_row(buf);
}


template<typename Point, typename Allocator>
std::size_t range_point_cloud<Point, Allocator>::width() const {
	return image_.size()[0];
}


template<typename Point, typename Allocator>
std::size_t range_point_cloud<Point, Allocator>::height() const {
	return image_.size()[1];
}



}
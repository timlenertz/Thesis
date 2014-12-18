#include <cmath>

namespace pcf {

template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(range_point_cloud_importer& imp, const Allocator& alloc) :
super(imp.size(), false, alloc),
image_({imp.columns(), imp.rows()}, super::begin_, super::begin_ + imp.size()) {
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


template<typename Point, typename Allocator>
range_image range_point_cloud<Point, Allocator>::to_range_image() const {
	range_image ri(width(), height());
	for(auto it = image_.begin(); it != image_.end(); ++it) {
		auto ind = it.index();
		if(it->valid()) ri.at(ind[0], ind[1]) = it->coordinates().norm();
		else ri.invalidate(ind[0], ind[1]);
	}
	return ri;
}


template<typename Point, typename Allocator>
color_image range_point_cloud<Point, Allocator>::to_color_image(rgb_color bg) const {
	color_image ci(width(), height());
	for(auto it = image_.begin(); it != image_.end(); ++it) {
		auto ind = it.index();
		rgb_color& col = ci.at(ind[0], ind[1]);
		if(it->valid()) col = it->color;
		else col = bg;
	}
	return ci;
}



}
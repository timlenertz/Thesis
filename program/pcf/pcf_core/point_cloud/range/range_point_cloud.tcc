#include <cmath>
#include "../../image/range_image.h"
#include "../../image/color_image.h"
#include "../../io/range_point_cloud_importer.h"

namespace pcf {

template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(std::size_t w, std::size_t h, bool row_major, const Allocator& alloc) :
	super(w * h, alloc),
	image_(row_major ? size_2dim(h, w) : size_2dim(w, h), super::begin_, super::begin_ + super::capacity()),
	row_major_order_(row_major) { }



template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(range_point_cloud_importer& imp, const Allocator& alloc) :
	range_point_cloud(imp.columns(), imp.rows(), true, alloc)
{
	if(imp.has_camera_pose())
		super::set_relative_pose( imp.camera_pose() );

	super::resize_(super::capacity());
	for(Point* buf = super::begin_; buf < super::end_; buf += imp.columns())
		imp.read_row(buf);
}


template<typename Point, typename Allocator>
std::size_t range_point_cloud<Point, Allocator>::width() const {
	return image_.size()[row_major_order_ ? 1 : 0];
}


template<typename Point, typename Allocator>
std::size_t range_point_cloud<Point, Allocator>::height() const {
	return image_.size()[row_major_order_ ? 0 : 1];
}


template<typename Point, typename Allocator>
const Point& range_point_cloud<Point, Allocator>::at(std::ptrdiff_t x, std::ptrdiff_t y) const {
	if(row_major_order_) return image_[{ y, x }];
	else return image_[{ x, y }];
}



template<typename Point, typename Allocator>
Point& range_point_cloud<Point, Allocator>::at(std::ptrdiff_t x, std::ptrdiff_t y) {
	if(row_major_order_) return image_[{ y, x }];
	else return image_[{ x, y }];
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
		if(it->valid()) col = it->get_color();
		else col = bg;
	}
	return ci;
}


}
#include <cmath>
#include <utility>
#include "../../image/range_image.h"
#include "../../image/color_image.h"
#include "../../io/range_point_cloud_importer.h"

namespace pcf {

template<typename Point, typename Allocator>
multi_dimensional_buffer<Point, 2> range_point_cloud<Point, Allocator>::create_image_(bool row_major, std::size_t w, std::size_t h, Point* buf) {
	size_2dim sz;
	if(row_major) sz = size_2dim(h, w);
	else sz = size_2dim(w, h);
	std::size_t len = w * h;
	return multi_dimensional_buffer<Point, 2>(sz, buf, buf + len);
}

template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(std::size_t w, std::size_t h, bool row_major, const Allocator& alloc) :
	super(w * h, alloc),
	image_(create_image_(row_major, w, h, super::begin_)),
	row_major_order_(row_major)
{
	super::resize_(super::capacity());
}



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


template<typename Point, typename Allocator> template<typename Other_point, typename Other_allocate>
range_point_cloud<Point, Allocator>::range_point_cloud(const range_point_cloud<Other_point, Other_allocate>& pc, const Allocator& alloc) :
	super(pc, 0, alloc),
	image_(create_image_(pc.row_major_order(), pc.width(), pc.height(), super::begin_)),
	row_major_order_(pc.row_major_order()) { }


template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(range_point_cloud&& pc) :
	super(std::move(pc)),
	image_(pc.image_),
	row_major_order_(pc.row_major_order()) { }



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



template<typename Point, typename Allocator>
void range_point_cloud<Point, Allocator>::colorize(const color_image& im) {
	if( (im.width() != width()) || (im.height() != height()) )
		throw std::invalid_argument("Color image must have same size as range point cloud.");
	
	for(std::ptrdiff_t y = 0; y < im.height(); ++y)
	for(std::ptrdiff_t x = 0; x < im.width(); ++x) {
		Point& p = at(x, y);
		if(p.valid()) {
			rgb_color col = im.at(x, y);
			p.set_color(col);
		}
	}
}


}
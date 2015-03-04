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


template<typename Point, typename Allocator>
range_image_camera range_point_cloud<Point, Allocator>::estimate_range_camera() const {
	/*
	Attempt to model the camera that yielded this range point cloud. Can only get approximate model, because
	the range point cloud X-Y grid may not correspond to spherical coordinates as used by range_image_camera.
	Input: this point set with XYZ spatial coordinates of the points, and corresponding XY image coordinates
	Output: pose and parameters of a range camera that maps the XYZ coords to these XY coords as closely as possible
	*/
	
	// Assume the camera is at position 0, 0, 0.
	// Need its orientation, field of view angles and image size.
	
	struct mapping {
		spherical_coordinates spatial;
		index_2dim pixel;
	};
	
	// Find minimal and maximal angles
	angle min_az = -INFINITY, max_az = INFINITY, min_el = -INFINITY, max_el = INFINITY;
	for(const Point& pt : *this) {
		spherical_coordinates s = spherical_coordinates::from_cartesian(pt);
		if(s.azimuth > max_az) max_az = s.azimuth;
		else if(s.azimuth < min_az) min_az = s.azimuth;
		if(s.elevation > max_el) max_el = s.elevation;
		else if(s.elevation < min_el) min_el = s.elevation;
	}
	
	
}



}
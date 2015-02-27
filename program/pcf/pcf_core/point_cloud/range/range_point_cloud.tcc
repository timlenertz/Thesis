#include <cmath>
#include "../../image/range_image.h"
#include "../../image/color_image.h"
#include "../../io/range_point_cloud_importer.h"

namespace pcf {

template<typename Point, typename Allocator>
range_point_cloud<Point, Allocator>::range_point_cloud(range_point_cloud_importer& imp, const Allocator& alloc) :
	super(imp.size(), false, alloc),
	image_({imp.rows(), imp.columns()}, super::begin_, super::begin_ + imp.size())
{
	if(imp.has_camera_pose())
		super::set_relative_pose( imp.camera_pose() );

	super::resize_(super::capacity());
	for(Point* buf = super::begin_; buf < super::end_; buf += imp.columns())
		imp.read_row(buf);
}


template<typename Point, typename Allocator> template<typename Other_cloud, typename Camera>
range_point_cloud<Point, Allocator>::range_point_cloud(Other_cloud&& pc, const Camera& cam, const Allocator& alloc) :
	super(cam.image_number_of_pixels(), false, alloc),
	image_({cam.image_width(), cam.image_height()}, super::begin_, super::begin_ + cam.image_number_of_pixels())
{
	super::resize_(super::capacity());
	
	// Project pc onto this depth map using image camera cam.
	// Uses z-buffer to keep only point with highest depth value.
	
	multi_dimensional_array<float, 2> z_buffer(image_.size(), INFINITY);
	
	#pragma omp parallel for
	for(auto p = pc.cbegin(); p < pc.cend(); ++p) {
		if(! p->valid()) continue;
		
		float z;
		auto ic = cam.to_image(*p, z);
		if(! cam.in_bounds(ic)) continue;
		
		float& old_z = z_buffer[{ic[0], ic[1]}];
		if(z < old_z) {
			old_z = z;
			image_[{ic[0], ic[1]}] = *p;
		}
	}
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
		if(it->valid()) col = it->get_color();
		else col = bg;
	}
	return ci;
}



}
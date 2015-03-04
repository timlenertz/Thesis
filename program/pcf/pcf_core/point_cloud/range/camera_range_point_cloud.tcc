#include <stdexcept>
#include <iostream>

namespace pcf {

template<typename Point, typename Image_camera, typename Allocator> template<typename Iterator>
void camera_range_point_cloud<Point, Image_camera, Allocator>::project_(Iterator begin, Iterator end) {
	// Project pc onto this depth map using image camera cam.
	// Uses z-buffer to keep only point with highest depth value.

	array_2dim<float> z_buffer(super::image_.size(), INFINITY);
	
	#pragma omp parallel for
	for(auto p = begin; p < end; ++p) {
		if(! p->valid()) continue;
		
		auto ic = camera_.to_image(*p);
		if(! camera_.in_bounds(ic)) continue;
		float z = camera_.depth(*p);

		float& old_z = z_buffer[{ic[0], ic[1]}];
		if(z < old_z) {
			old_z = z;
			super::image_[{ic[0], ic[1]}] = *p;
		}
	}
}


template<typename Point, typename Image_camera, typename Allocator>
camera_range_point_cloud<Point, Image_camera, Allocator>::camera_range_point_cloud(const point_cloud_xyz& pc, const Image_camera& cam, const Allocator& alloc) :
	super(cam.image_width(), cam.image_height(), false, alloc),
	camera_(cam)
{
	super::resize_(super::capacity());
	super::initialize_();
	project_(pc.cbegin(), pc.cend());
}


template<typename Point, typename Image_camera, typename Allocator>
camera_range_point_cloud<Point, Image_camera, Allocator>::camera_range_point_cloud(const point_cloud_full& pc, const Image_camera& cam, const Allocator& alloc) :
	super(cam.image_width(), cam.image_height(), false, alloc),
	camera_(cam)
{
	super::resize_(super::capacity());
	super::initialize_();
	project_(pc.cbegin(), pc.cend());
}


template<typename Point, typename Image_camera, typename Allocator>
camera_range_point_cloud<Point, Image_camera, Allocator>::camera_range_point_cloud(const range_image& ri, const Image_camera& cam, const Allocator& alloc) :
	super(ri.width(), ri.height(), false, alloc),
	camera_(cam)
{
	if( (ri.width() != cam.image_width()) || (ri.height() != cam.image_height()) )
		throw std::invalid_argument("Camera image size and range image size don't match.");

	super::resize_(super::capacity());
		
	for(auto it = super::image_.begin(); it != super::image_.end(); ++it) {
		auto ind = it.index();
		if(ri.valid(ind[0], ind[1]))
			*it = camera_.point(ind, ri.at(ind[0], ind[1]));
		else
			it->invalidate();
	}	
}

}
#include <stdexcept>
#include <iostream>

namespace pcf {

template<typename Point, typename Image_camera, typename Allocator> template<typename Other_cloud>
camera_range_point_cloud<Point, Image_camera, Allocator>::camera_range_point_cloud(const Other_cloud& pc, const Image_camera& cam, const Allocator& alloc) :
	super(cam.image_width(), cam.image_height(), false, alloc),
	camera_(cam)
{
	// Project pc onto this depth map using image camera cam.
	// Uses z-buffer to keep only point with highest depth value.
	
	super::resize_(super::capacity());
	super::initialize_();
	
	multi_dimensional_array<float, 2> z_buffer(super::image_.size(), INFINITY);
	
	#pragma omp parallel for
	for(auto p = pc.cbegin(); p < pc.cend(); ++p) {
		if(! p->valid()) continue;
		
		float z;
		auto ic = cam.to_image(*p, z);
		if(! cam.in_bounds(ic)) continue;

		float& old_z = z_buffer[{ic[0], ic[1]}];
		if(z < old_z) {
			old_z = z;
			super::image_[{ic[0], ic[1]}] = *p;
		}
	}
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
		std::ptrdiff_t x = it.index()[0], y = x = it.index()[1];
		if(ri.valid(x, y))
			*it = camera_.point(it.index(), ri.at(x, y));
		else
			it->invalidate();
	}	
}

}
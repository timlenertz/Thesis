#include <stdexcept>
#include <iostream>

namespace pcf {


template<typename Point, typename Image_camera, typename Allocator> template<typename Other_cloud>
camera_range_point_cloud<Point, Image_camera, Allocator>::camera_range_point_cloud(const Other_cloud& pc, const Image_camera& cam, const Allocator& alloc) :
	camera_range_point_cloud(cam, alloc)
{
	project(pc);
}

template<typename Point, typename Image_camera, typename Allocator> template<typename Iterator, typename Colorize_func>
void camera_range_point_cloud<Point, Image_camera, Allocator>::project_(Iterator begin, Iterator end, Colorize_func col, bool depth_test) {
	// Project pc onto this depth map using image camera cam.
	// Uses z-buffer to keep only point with highest depth value.

	array_2dim<float> z_buffer(super::image_.size(), INFINITY);
	if(depth_test) {
		for(std::ptrdiff_t y = 0; y < super::height(); ++y)
		for(std::ptrdiff_t x = 0; x < super::width(); ++x) {
			const Point& p = super::at(x, y);
			if(p.valid()) z_buffer[{x, y}] = p.coordinates().norm();
		}
	}
	
	auto view_transformation = camera_.view_transformation();
		
	#pragma omp parallel for if(0)
	for(auto it = begin; it < end; ++it) {	
		auto p = *it;
		if(! p.valid()) continue;
		
		auto ic = camera_.to_image(p);
				
		if(! camera_.in_bounds(ic)) continue;
		
		float z = camera_.depth(p);

		float& old_z = z_buffer[ic];
		if(z < old_z) {
			old_z = z;
			Point np = p;
			np.set_color(col(p));
			np.apply_transformation(view_transformation);
			super::image_[ic] = np;
		}
	}
}


template<typename Point, typename Image_camera, typename Allocator>
camera_range_point_cloud<Point, Image_camera, Allocator>::camera_range_point_cloud(const Image_camera& cam, const Allocator& alloc) :
	super(cam.image_width(), cam.image_height(), false, alloc),
	camera_(cam)
{
	super::resize_(super::capacity());
	super::initialize_();
		
	super::set_relative_pose(cam.absolute_pose());
	camera_.set_parent(*this, pose());
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


template<typename Point, typename Image_camera, typename Allocator>
void camera_range_point_cloud<Point, Image_camera, Allocator>::project(const point_cloud_xyz& pc, const rgb_color& col) {
	project_(pc.begin_relative_to(*this), pc.end_relative_to(), [col](const point_xyz& p) -> rgb_color { return col; });
}


template<typename Point, typename Image_camera, typename Allocator>
void camera_range_point_cloud<Point, Image_camera, Allocator>::project(const point_cloud_full& pc, const rgb_color& col) {
	project_(pc.begin_relative_to(*this), pc.end_relative_to(), [col](const point_full& p) -> rgb_color { return col; });
}


template<typename Point, typename Image_camera, typename Allocator>
void camera_range_point_cloud<Point, Image_camera, Allocator>::project(const point_cloud_full& pc) {
	project_(pc.begin_relative_to(*this), pc.end_relative_to(), [](const point_full& p) -> rgb_color { return p.get_color(); });
}

}
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "../../image/projected_range_image.h"

namespace pcf {


template<typename Other_cloud, typename Image_camera>
Image_camera estimate_optimal_camera(const Image_camera& original_cam, const Other_cloud& pc, std::size_t min_w, std::size_t max_w) {
	float n = pc.number_of_valid_points();
	auto sample = [&](std::size_t imw) -> float {
		Image_camera cam = original_cam;
		cam.set_image_width(imw);
		auto rpc = project(pc, cam);
		float filled = rpc.ratio_of_area_filled();
		float points = rpc.number_of_valid_points() / n;
		return (filled - points);
	};
	
	std::size_t lo = min_w, hi = max_w;
	std::size_t imw;
	for(;;) {
		imw = (lo + hi) / 2;
		float pivot_sample = sample(imw);
		if(std::abs(pivot_sample) < 0.03) break;
		else if(pivot_sample > 0) lo = imw;
		else if(pivot_sample < 0) hi = imw;	
	}
	
	Image_camera cam = original_cam;
	cam.set_image_width(imw);
	return cam;
}


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
		if(ri.valid(ind))
			*it = camera_.point(ind, ri[ind]);
		else
			it->invalidate();
	}	
}


template<typename Point, typename Image_camera, typename Allocator>
camera_range_point_cloud<Point, Image_camera, Allocator>::camera_range_point_cloud(const projected_range_image& ri, const Image_camera& cam, const Allocator& alloc) :
	super(ri.width(), ri.height(), false, alloc),
	camera_(cam)
{
	if( (ri.width() != cam.image_width()) || (ri.height() != cam.image_height()) )
		throw std::invalid_argument("Camera image size and range image size don't match.");

	super::resize_(super::capacity());
		
	for(auto it = super::image_.begin(); it != super::image_.end(); ++it) {
		auto ind = it.index();
		if(ri.valid(ind))
			*it = camera_.point_with_projected_depth(ind, ri[ind]);
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


template<typename Point, typename Image_camera, typename Allocator>
projected_range_image camera_range_point_cloud<Point, Image_camera, Allocator>::to_projected_range_image() const {
	projected_range_image ri(super::width(), super::height());
	for(auto it = super::image_.begin(); it != super::image_.end(); ++it) {
		auto ind = it.index();
		if(it->valid()) ri[ind] = camera_.projected_depth(*it);
		else ri.invalidate(ind);
	}
	return ri;
}


/////////////// 



template<typename Point, typename Image_camera, typename Allocator>
projected_range_image camera_range_point_cloud<Point, Image_camera, Allocator>::fill_holes(float background, float corner_tolerance) const {
	using namespace cv;

	// Make multi-scale stack
	std::stack<Mat> st;
	float mn = NAN, mx;
	std::size_t w = super::width(), h = super::height();
	
	std::size_t minimal_image_width = estimate_optimal_camera(camera_, *this).image_width();
		
	Image_camera cam = camera_;	
	for(;;) {
		projected_range_image ri = camera_range_point_cloud(*this, cam).to_projected_range_image();
		if(std::isnan(mn)) std::tie(mn, mx) = ri.minimum_and_maximum();

		Mat& mat = ri;
		mat.setTo(background, ri.mask());
		st.push(mat);
		
		if(w < minimal_image_width) break;
		
		w /= 2; h /= 2;
		cam.set_image_size(w, h);
	}
	
	// Merge using the stack
	Mat merged = st.top();
	
	st.pop();
	while(st.size()) {
		Mat& hi = st.top();
		
		// Enlarge LO to size of HI using bilinear filtering
		resize(merged, merged, hi.size(), 0, 0, INTER_LINEAR);
		
		// Detect edges in resized LO, fill with pixels from HI
		Mat corners(merged.size(), CV_8UC1);
		Sobel(merged, corners, -1, 1, 1, 3);
		unsigned char min_corner = corner_tolerance * 225.0;
		corners = (corners > min_corner) & (hi != background);
		hi.copyTo(merged, corners);
		
		// Merge using minimum function (i.e. keep closest)
		merged = min(hi, merged);
		
		st.pop();
	}

	merged.setTo(mn, (merged < mn));
	merged.setTo(mx, (merged > mx) & (merged != background));

	projected_range_image ri(merged);
	ri.mask().setTo(0, (merged == background));
	return ri;
}

}
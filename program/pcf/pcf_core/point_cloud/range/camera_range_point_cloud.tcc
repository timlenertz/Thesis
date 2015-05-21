#include <stdexcept>
#include <iostream>
#include <opencv2/opencv.hpp>

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
camera_range_point_cloud<Point, Image_camera, Allocator>::camera_range_point_cloud(const range_image& ri, const Image_camera& cam, bool projected_depth, const Allocator& alloc) :
	super(ri.width(), ri.height(), false, alloc),
	camera_(cam)
{
	if( (ri.width() != cam.image_width()) || (ri.height() != cam.image_height()) )
		throw std::invalid_argument("Camera image size and range image size don't match.");

	super::resize_(super::capacity());
		
	for(auto it = super::image_.begin(); it != super::image_.end(); ++it) {
		auto ind = it.index();
		if(ri.valid(ind[0], ind[1]))
			if(projected_depth) *it = camera_.point_with_projected_depth(ind, ri.at(ind[0], ind[1]));
			else *it = camera_.point(ind, ri.at(ind[0], ind[1]));
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
range_image camera_range_point_cloud<Point, Image_camera, Allocator>::to_range_image(bool projected_depth) const {
	if(projected_depth) {
		range_image ri(super::width(), super::height());
		for(auto it = super::image_.begin(); it != super::image_.end(); ++it) {
			auto ind = it.index();
			if(it->valid()) ri.at(ind[0], ind[1]) = camera_.projected_depth(*it);
			else ri.invalidate(ind[0], ind[1]);
		}
		return ri;
	} else {
		return super::to_range_image();
	}
}


/////////////// 



template<typename Point, typename Image_camera, typename Allocator>
range_image camera_range_point_cloud<Point, Image_camera, Allocator>::fill_holes() {
	using namespace cv;

	// Make multi-scale stack
	std::stack<Mat> st;
	float mn = NAN, mx;
	std::size_t w = super::width(), h = super::height();
	
	const float background = 200;

	Image_camera cam = camera_;	
	do {
		range_image ri = camera_range_point_cloud(*this, cam).to_range_image(true);
		bool holes = ri.contains_holes();
		if(std::isnan(mn)) std::tie(mn, mx) = ri.minimum_and_maximum();

		Mat& mat = ri;
		mat.setTo(background, (mat == range_image::invalid_value));
		st.push(mat);
		
		w /= 2; h /= 2;
		cam.set_image_size(w, h);
		if(! holes) break;
	} while(w > 1 && h > 1);

	
	// Merge using the stack
	Mat merged = st.top();
	
	
	st.pop();
	
	while(st.size()) {
		Mat& hi = st.top();
		
		// Enlarge LO to size of HI using bilinear filtering
		resize(merged, merged, hi.size(), 0, 0, INTER_LINEAR);
		//merged.setTo(mn, (merged < mn));
		//merged.setTo(mx, (merged > mx) & (merged != background));
		
		// Detect edges in resized LO, fill with pixels from HI
		Mat corners(merged.size(), CV_8UC1);
		Sobel(merged, corners, -1, 1, 1, 3);
		corners = (corners > 100) & (hi != background);
		hi.copyTo(merged, corners);
		
		// Merge using minimum function (i.e. keep closest)
		merged = min(hi, merged);
		
		st.pop();
	}

		merged.setTo(mn, (merged < mn));
		merged.setTo(mx, (merged > mx) & (merged != background));

	merged.setTo(range_image::invalid_value, (merged == background));
	return range_image(merged);
}

}
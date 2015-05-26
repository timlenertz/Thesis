#ifndef PCF_CAMERA_RANGE_POINT_CLOUD_H_
#define PCF_CAMERA_RANGE_POINT_CLOUD_H_

#include "range_point_cloud.h"
#include <memory>
#include <stack>

namespace pcf {

class range_image;
class projected_range_image;

/**
Range point cloud with associated image camera.
The camera defined the exact mapping from spatial point coordinates to image pixel coordinates. Mapping from image coordinates to spacial coordinates is defined by the point cloud's structure.
*/
template<typename Point, typename Image_camera, typename Allocator = default_allocator<Point>>
class camera_range_point_cloud : public range_point_cloud<Point, Allocator> {
	using super = range_point_cloud<Point, Allocator>;

private:
	Image_camera camera_;
	
	template<typename Iterator, typename Colorize_func>
	void project_(Iterator begin, Iterator end, Colorize_func, bool depth_test = true);

public:
	camera_range_point_cloud(const Image_camera&, const Allocator& alloc = Allocator());

	template<typename Other_cloud>
	camera_range_point_cloud(const Other_cloud& pc, const Image_camera& cam, const Allocator& alloc = Allocator());

	/// Create from existing range image with given image camera.
	/// Range image size must be same as image camera image size.
	camera_range_point_cloud(const range_image&, const Image_camera&, const Allocator& alloc = Allocator());
	camera_range_point_cloud(const projected_range_image&, const Image_camera&, const Allocator& alloc = Allocator());

	void project(const point_cloud_xyz&, const rgb_color& col = point_xyz::default_color);
	void project(const point_cloud_full&, const rgb_color& col);
	void project(const point_cloud_full&);

	projected_range_image to_projected_range_image() const;

	projected_range_image fill_holes(float background_depth = 1000.0, float corner_tolerance = 0.2) const;

	// TODO copy, move constructor
};


extern template class camera_range_point_cloud<point_xyz, projection_image_camera>;
extern template class camera_range_point_cloud<point_full, projection_image_camera>;

template<typename Image_camera> using camera_range_point_cloud_xyz = camera_range_point_cloud<point_xyz, Image_camera>;
template<typename Image_camera> using camera_range_point_cloud_full = camera_range_point_cloud<point_full, Image_camera>;

template<typename Image_camera>
camera_range_point_cloud_xyz<Image_camera> project(const point_cloud_xyz& pc, const Image_camera& cam) {
	return camera_range_point_cloud_xyz<Image_camera>(pc, cam);
}

template<typename Image_camera>
camera_range_point_cloud_full<Image_camera> project(const point_cloud_full& pc, const Image_camera& cam) {
	return camera_range_point_cloud_full<Image_camera>(pc, cam);
}

template<typename Image_camera>
camera_range_point_cloud_xyz<Image_camera> backproject(const range_image& ri, const Image_camera& cam) {
	return camera_range_point_cloud_xyz<Image_camera>(ri, cam);
}


template<typename Image_camera>
camera_range_point_cloud_xyz<Image_camera> backproject(const projected_range_image& ri, const Image_camera& cam) {
	return camera_range_point_cloud_xyz<Image_camera>(ri, cam);
}


template<typename Image_camera>
camera_range_point_cloud_full<Image_camera> backproject(const range_image& ri, const color_image& ci, const Image_camera& cam) {
	camera_range_point_cloud_full<Image_camera> rpc(ri, cam);
	rpc.colorize(ci);
	return rpc;
}


template<typename Other_cloud, typename Image_camera>
Image_camera estimate_optimal_camera(const Image_camera& original_cam, const Other_cloud& pc, std::size_t min_w = 10, std::size_t max_w = 3000);


}

#include "camera_range_point_cloud.tcc"

#endif

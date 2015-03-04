#ifndef PCF_CAMERA_RANGE_POINT_CLOUD_H_
#define PCF_CAMERA_RANGE_POINT_CLOUD_H_

#include "range_point_cloud.h"
#include <memory>

namespace pcf {

class range_image;

/**
Range point cloud with associated image camera.
The camera defined the exact mapping from spatial point coordinates to image pixel coordinates. Mapping from image coordinates to spacial coordinates is defined by the point cloud's structure.
*/
template<typename Point, typename Image_camera, typename Allocator = default_allocator<Point>>
class camera_range_point_cloud : public range_point_cloud<Point, Allocator> {
	using super = range_point_cloud<Point, Allocator>;

private:
	Image_camera camera_;
	
	template<typename Iterator>
	void project_(Iterator begin, Iterator end);

public:
	/// Create by projection of existing point cloud with given image camera.
	camera_range_point_cloud(const point_cloud_xyz& pc, const Image_camera&, const Allocator& alloc = Allocator());

	/// Create by projection of existing point cloud with given image camera.
	camera_range_point_cloud(const point_cloud_full& pc, const Image_camera&, const Allocator& alloc = Allocator());
	
	/// Create from existing range image with given image camera.
	/// Range image size must be same as image camera image size.
	camera_range_point_cloud(const range_image&, const Image_camera&, const Allocator& alloc = Allocator());
};


extern template class camera_range_point_cloud<point_xyz, range_image_camera>;
extern template class camera_range_point_cloud<point_xyz, projection_image_camera>;
extern template class camera_range_point_cloud<point_full, range_image_camera>;
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

}

#include "camera_range_point_cloud.tcc"

#endif
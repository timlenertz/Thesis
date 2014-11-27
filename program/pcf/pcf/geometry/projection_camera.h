#ifndef PCF_PROJECTION_CAMERA_H_
#define PCF_PROJECTION_CAMERA_H_

#include "camera.h"
#include <cmath>

namespace pcf {

/**
Camera which additionally handles projection to image space.
Contains definition of image width and height in pixel, and provides projection onto image space. Matrix is computed using X field of view and image aspect ratio, instead of using two angles; avoiding unnecessary trigonometric operations. After construction, field of view angles can be altered, and then image size may need to be readjusted to fix aspect ratio and avoid distortion. (Same for image size and then fov angles).
*/
class projection_camera : public camera {
public:
	using image_coordinates = std::array<std::ptrdiff_t, 2>;
	using image_size = std::array<std::size_t, 2>;

private:
	image_size image_size_;
	image_coordinates image_center_;
	
	static angle compute_fov_y_(angle fov_x, image_size);
	static angle compute_fov_x_(angle fov_y, image_size);

	void compute_transformations_using_aspect_ratio_();

public:
	projection_camera(const pose&, angle fov_x, std::size_t imw, std::size_t imh, float znear = default_z_near_, float zfar = default_z_far_);
	
	void set_image_size(std::size_t imw, std::size_t imh);

	void adjust_field_of_view(bool keep_x = true);
	void adjust_image_size(bool keep_width = true);

	image_coordinates to_projected(const Eigen::Vector3f&) const;
	spherical_coordinates projected_to_spherical(const image_coordinates&, float distance = NAN) const;	
	Eigen::Vector3f point(const image_coordinates&, float z = 0) const;	
};

}

#endif
#ifndef PCF_IMAGE_CAMERA_H_
#define PCF_IMAGE_CAMERA_H_

#include "camera.h"
#include <cmath>

namespace pcf {

/**
Camera that additionally handles projection to image.
Stored pixel width and height of the image.
*/
class image_camera : public camera {
public:
	using image_coordinates = std::array<std::ptrdiff_t, 2>;

	std::size_t image_width;
	std::size_t image_height;

	projection_camera(const pose&, const projection_frustum&, std::size_t imw);
	projection_camera(const camera&, std::size_t imw);
	projection_camera(const projection_camera&) = default;
	
	float aspect_ratio() const;
	
	image_coordinates to_image(const Eigen::Vector3f&) const;
	spherical_coordinates image_to_spherical(const image_coordinates&, float distance = NAN) const;	
	Eigen::Vector3f point(const image_coordinates&, float z = 0) const;	
};

}

#endif
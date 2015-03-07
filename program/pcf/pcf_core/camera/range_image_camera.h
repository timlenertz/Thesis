#ifndef PCF_RANGE_IMAGE_CAMERA_H_
#define PCF_RANGE_IMAGE_CAMERA_H_

#include "range_camera.h"
#include "image_camera.h"
#include <array>

namespace pcf {

/**
Range camera which additionally handles projection onto pixel grid.
Image pixel XY coordinates linearily map to angles, aka projection on sphere. This models the 3D scanner that has produced a range point cloud.
*/
class range_image_camera : public range_camera, public image_camera {
public:
	range_image_camera() = default;
	range_image_camera(const pose&, angle width, angle height, std::size_t imw, std::size_t imh);
	range_image_camera(const pose&, std::array<angle, 2> x_limits, std::array<angle, 2> y_limits, std::size_t imw, std::size_t imh);
	range_image_camera(const range_image_camera&) = default;

	angle angular_resolution_x() const;
	angle angular_resolution_y() const;
	
	image_coordinates to_image(const Eigen::Vector3f&) const override;
	Eigen::Vector3f point(image_coordinates, float depth) const override;
};

}

#endif

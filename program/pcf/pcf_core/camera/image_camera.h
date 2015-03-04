#ifndef PCF_IMAGE_CAMERA_H_
#define PCF_IMAGE_CAMERA_H_

#include <cstddef>
#include <array>
#include <Eigen/Eigen>
#include "../util/coordinates.h"

namespace pcf {

/**
Abstract (secondary) base class for camera which handles mapping to image coordinates.
*/
class image_camera {
protected:
	std::size_t image_width_;
	std::size_t image_height_;
	
	image_camera() = default;
	image_camera(std::size_t imw, std::size_t imh);
	
public:
	using image_coordinates = index_2dim;
	
	image_camera(const image_camera&) = default;
	
	float image_aspect_ratio() const;
	std::size_t image_width() const { return image_width_; }
	std::size_t image_height() const { return image_height_; }
	std::size_t image_number_of_pixels() const;
	void set_image_size(std::size_t imw, std::size_t imh);
	
	virtual image_coordinates to_image(const Eigen::Vector3f&, float& z) const = 0;
	image_coordinates to_image(const Eigen::Vector3f&) const;
	virtual Eigen::Vector3f point(image_coordinates, float depth) const = 0;
	
	bool in_bounds(image_coordinates) const;
};

}

#endif
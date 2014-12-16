#ifndef PCF_IMAGE_CAMERA_H_
#define PCF_IMAGE_CAMERA_H_

#include <cstddef>
#include <array>
#include <Eigen/Eigen>

namespace pcf {

/**
Abstract (secondary) base class for camera which handles mapping to image coordinates.
*/
class image_camera {
protected:
	std::size_t image_width_;
	std::size_t image_height_;
	
	image_camera(std::size_t imw, std::size_t imh);
	
public:
	using image_coordinates = std::array<std::ptrdiff_t, 2>;
	
	image_camera(const image_camera&) = default;
	
	float image_aspect_ratio() const;
	std::size_t image_width() const { return image_width_; }
	std::size_t image_height() const { return image_height_; }
	void set_image_size(std::size_t imw, std::size_t imh);
	
	virtual image_coordinates to_image(const Eigen::Vector3f&) const = 0;
	virtual Eigen::Vector3f point(image_coordinates, float depth) const = 0;
};

}

#endif
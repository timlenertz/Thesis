#ifndef PCF_IMAGE_CAMERA_H_
#define PCF_IMAGE_CAMERA_H_

#include <cstddef>
#include <array>
#include <Eigen/Eigen>
#include "../util/coordinates.h"

namespace pcf {

/**
Abstract (secondary) base class for camera which handles mapping to image coordinates.
Image pixels may be graduation of projection plane (projection camera), or or spherical coordinates (range camera).
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
	size_2dim image_size() const { return size_2dim(image_width_, image_height_); }
	std::size_t image_number_of_pixels() const;
	void set_image_size(std::size_t imw, std::size_t imh);
	
	/// Set image width, and adjust height to keep same aspect ratio.
	void set_image_width(std::size_t);
	
	/// Set image height, and adjust width to keep same aspect ratio.
	void set_image_height(std::size_t);
	
	/// Check whether given coordinates are in image bounds.
	bool in_bounds(image_coordinates) const;
	
	/// Map point in space to image coordinates.
	/// Implemented by subclass.
	virtual image_coordinates to_image(const Eigen::Vector3f&) const = 0;
	
	/// Inverse mapping of image coordinates to point in space.
	/// Depth is the distance of the point to the camera origin.
	/// projection_image_camera also provides a version which takes the projected depth instead.
	virtual Eigen::Vector3f point(image_coordinates, float depth) const = 0;
};

}

#endif
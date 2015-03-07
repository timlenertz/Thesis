#include "range_image_camera.h"

namespace pcf {

range_image_camera::range_image_camera(const pose& ps, std::array<angle, 2> x_limits, std::array<angle, 2> y_limits, std::size_t imw, std::size_t imh) :
	range_camera(ps, x_limits, y_limits),
	image_camera(imw, imh) { }


angle range_image_camera::angular_resolution_x() const {
	return field_of_view_width() / image_width_;
}

angle range_image_camera::angular_resolution_y() const {
	return field_of_view_height() / image_height_;
}

auto range_image_camera::to_image(const Eigen::Vector3f& p) const -> image_coordinates {
	spherical_coordinates s = to_spherical(p);
	s.azimuth -= azimuth_limits_[0];
	s.elevation -= elevation_limits_[0];
	std::ptrdiff_t x = (s.azimuth * image_width_) / field_of_view_width();
	std::ptrdiff_t y = (s.elevation * image_height_) / field_of_view_height();
	return image_coordinates(x, y);
}

Eigen::Vector3f range_image_camera::point(image_coordinates im, float depth) const {
	spherical_coordinates s;
	s.azimuth = azimuth_limits_[0] + ((field_of_view_width() * im.x) / image_width_);
	s.elevation = azimuth_limits_[1] + ((field_of_view_height() * im.y) / image_height_);
	s.radius = depth;
	return camera::point(s);
}


}

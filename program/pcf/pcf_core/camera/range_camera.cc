#include "range_camera.h"
#include "../geometry/math_constants.h"

namespace pcf {

range_camera::range_camera(const pose& ps, angle width, angle height) :
	camera(ps),
	azimuth_limits_({ -width/2.0f, +width/2.0f }),
	elevation_limits_({ -height/2.0f, +height/2.0f }) { }	


range_camera::range_camera(const pose& ps, std::array<angle, 2> x_limits, std::array<angle, 2> y_limits) :
	camera(ps),
	azimuth_limits_(x_limits),
	elevation_limits_(y_limits) { }


range_camera::range_camera(const camera& cam) :
	range_camera(cam.absolute_pose(), cam.field_of_view_limits_x(), cam.field_of_view_limits_y()) { }


angle range_camera::field_of_view_width() const {
	return azimuth_limits_[1] - azimuth_limits_[0];
}


angle range_camera::field_of_view_height() const {
	return elevation_limits_[1] - elevation_limits_[0];
}


std::array<angle, 2> range_camera::field_of_view_limits_x() const {
	return azimuth_limits_;
}


std::array<angle, 2> range_camera::field_of_view_limits_y() const {
	return elevation_limits_;
}


bool range_camera::in_field_of_view(const Eigen::Vector3f& p) const {
	spherical_coordinates s = to_spherical(p);
	return (s.azimuth >= azimuth_limits_[0]) && (s.azimuth <= azimuth_limits_[1])
	    && (s.elevation >= elevation_limits_[0]) && (s.elevation <= elevation_limits_[1]);
}


bool range_camera::has_viewing_frustum() const {
	return (field_of_view_width() < pi) && (field_of_view_height() < pi);
}


projection_frustum range_camera::relative_viewing_frustum() const {
	return projection_frustum::asymmetric_perspective_fov(
		field_of_view_limits_x(),
		field_of_view_limits_y()
	);
}


Eigen::Projective3f range_camera::projection_transformation() const {
	return Eigen::Projective3f(relative_viewing_frustum().matrix);
}


}
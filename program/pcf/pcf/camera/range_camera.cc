#include "range_camera.h"
#include "../geometry/math_constants.h"

namespace pcf {


angle range_camera::field_of_view_x() const {
	return azimuth_limits_[1] - azimuth_limits_[0];
}


angle range_camera::field_of_view_y() const {
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
	return (field_of_view_x() < pi) && (field_of_view_y() < pi);
}


projection_frustum range_camera::relative_viewing_frustum() const {
	return projection_frustum::asymmetric_perspective_fov(
		field_of_view_limits_x(),
		field_of_view_limits_y(),
	);
}


Eigen::Projective3f range_camera::projection_transformation() const {
	return Eigen::Projective3f(relative_viewing_frustum().matrix);
}


}
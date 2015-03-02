#include "camera.h"

namespace pcf {

camera::camera(const pose& ps) :
	space_object(ps) { }

	
angle camera::field_of_view_width() const {
	auto limits = this->field_of_view_limits_x();
	return limits[1] - limits[0];
}


angle camera::field_of_view_height() const {
	auto limits = this->field_of_view_limits_y();
	return limits[1] - limits[0];
}


frustum camera::viewing_frustum() const {
	return this->relative_viewing_frustum().transform(view_transformation());
}

Eigen::Affine3f camera::view_transformation() const {
	return absolute_pose().transformation_from_world();
}

Eigen::Projective3f camera::view_projection_transformation() const {
	return this->projection_transformation() * view_transformation();
}

Eigen::Vector3f camera::view_ray_direction() const {
	return absolute_pose().orientation * Eigen::Vector3f(0, 0, -1);
}

float camera::depth_sq(const Eigen::Vector3f& p) const {
	return (absolute_pose().position - p).squaredNorm();
}

float camera::depth(const Eigen::Vector3f& p) const {
	return (absolute_pose().position - p).norm();
}

spherical_coordinates camera::to_spherical(const Eigen::Vector3f& p) const {
	return spherical_coordinates::from_cartesian(view_transformation() * p);
}

Eigen::Vector3f camera::point(const spherical_coordinates& s) const {
	return absolute_pose().transformation_to_world() * s.to_cartesian();
}


}
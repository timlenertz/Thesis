#include "camera.h"
#include <cassert>
#include <cmath>

namespace pcf {


camera::camera(const pose& ps, const projection_frustum& fr) :
	pose_(ps), frustum_(fr) { }
	
	
Eigen::Affine3f camera::view_transformation() const {
	return pose_.view_transformation();
}

Eigen::Projective3f camera::projection_transformation() const {
	return Eigen::Projective3f(frustum_.matrix);
}

Eigen::Projective3f camera::view_projection_transformation() const {
	return projection_transformation() * view_transformation();
}

const projection_frustum& camera::relative_viewing_frustum() const {
	return frustum_;
}

frustum camera::viewing_frustum() const {
	return frustum_.transform(view_transformation());
}

const pose& camera::camera_pose() const {
	return pose_;
}

float camera::depth_sq(const Eigen::Vector3f& p) const {
	return (p - pose_.position).squaredNorm();
}

float camera::depth(const Eigen::Vector3f&) const {
	return (p - pose_.position).norm();
}

bool camera::in_field_of_view(const Eigen::Vector3f& p) const {
	return viewing_frustum().contains(p);
}

spherical_coordinates camera::to_spherical(const Eigen::Vector3f& p) const {
	return spherical_coordinates::from_cartesian(view_transformation() * p);
}

Eigen::Vector3f camera::point(const spherical_coordinates& s) const {
	return camera_pose.view_transformation_inverse() * s.to_cartesian();
}


}

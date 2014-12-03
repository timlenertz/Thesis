#include "camera.h"
#include "frustum.h"
#include <cassert>
#include <cmath>
#include <iostream>

namespace pcf {

const float camera::default_z_near_ = 0.001;
const float camera::default_z_far_ = 10000.0;

camera::camera(const pose& ps, angle fvx, angle fvy, float znear, float zfar, bool compute) :
pose_(ps), fov_x_(fvx), fov_y_(fvy), near_z_(znear), far_z_(zfar) {
	if(compute) compute_frustum_();
}


void camera::compute_frustum_() {
	float dz = far_z_ - near_z_;	
	float tanx = std::tan(fov_x_ / 2);
	float tany = std::tan(fov_x_ / 2);

	Eigen::Matrix4f projection_matrix;
	projection_matrix <<
		tanx, 0, 0, 0,
		0, tany, 0, 0,
		0, 0, -(far_z_ + near_z_)/dz, (-2.0f*near_z_*far_z_)/dz,
		0, 0, -1, 0;
				
	Eigen::Matrix4f view_projection_matrix = projection_matrix * pose_.view_transformation().matrix();
	viewing_frustum_ = frustum(view_projection_matrix);
}


const frustum& camera::viewing_frustum() const {
	return viewing_frustum_;
}


Eigen::Affine3f camera::view_transformation() const {
	return pose_.view_transformation();
}


Eigen::Affine3f camera::view_transformation_inverse() const {
	return view_transformation().inverse();
}


Eigen::Projective3f camera::view_projection_transformation() const {
	return Eigen::Projective3f(viewing_frustum_.view_projection_matrix);
}


Eigen::Projective3f camera::view_projection_transformation_inverse() const {
	return Eigen::Projective3f(viewing_frustum_.view_projection_matrix).inverse();
}



float camera::distance_sq(const Eigen::Vector3f& wp) const {
	return (pose_.position - wp).squaredNorm();
}


float camera::distance(const Eigen::Vector3f& wp) const {
	return (pose_.position - wp).norm();
}


bool camera::in_field_of_view(const Eigen::Vector3f& wp, bool consider_z_planes) const {
	return viewing_frustum_.contains(wp, consider_z_planes);
}


void camera::set_pose(const pose& ps) {
	pose_ = ps;
	compute_frustum_();
}


void camera::set_field_of_view(angle fov_x, angle fov_y) {
	fov_x_ = fov_x;
	fov_y_ = fov_y;
	compute_frustum_();
}



spherical_coordinates camera::to_spherical(const Eigen::Vector3f& wp) const {
	return spherical_coordinates::from_cartesian(view_transformation() * wp);
}


Eigen::Vector3f camera::point(const spherical_coordinates& s) const {
	return view_transformation_inverse() * s.to_cartesian();
}


}

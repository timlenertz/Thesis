#include "camera.h"
#include "frustum.h"
#include <cassert>
#include <cmath>

namespace pcf {

const float camera::default_z_near_ = 0.01;
const float camera::default_z_far_ = 100.0;

camera::camera(const pose& p, angle fvx, angle fvy, float znear, float zfar) :
pose_(p), fov_x_(fvx), fov_y_(fvy), near_z_(znear), far_z_(zfar) {
	compute_transformations_();
}


void camera::compute_transformations_() {
	view_ = pose_.view_transformation();
	view_inv_ = view_.inverse();

	float x_scale = 1.0f / std::tan(fov_x / 2);
	float y_scale = 1.0f / std::tan(fov_y / 2);
	float zdiff = far_z_ - near_z_;
	
	Eigen::Matrix4f mat;
	mat <<
		x_scale, 0, 0, 0,
		0, y_scale, 0, 0,
		0, 0, -(zfar + znear)/zdiff, -1,
		0, 0, -2*znear*zfar/zdiff, 0;
		
	view_projection_ = view_ * Eigen::Projective3f(mat);
	view_projection_inv_ = view_projection_.inverse();
}


frustum camera::viewing_frustum() const {
	frustum fr( view_projection_.matrix() );
	return fr;
}


float camera::distance_sq(const Eigen::Vector3f& wp) const {
	return (view_ * wp).squaredNorm();
}


float camera::distance(const Eigen::Vector3f& wp) const {
	return (view_ * wp).norm();
}


bool camera::in_field_of_view(const Eigen::Vector3f& wp, bool consider_z_planes) const {
	Eigen::Vector4f p = view_projection_ * wp.homogeneous();
	p /= p[3];
	bool inside = (p[0] >= -1) && (p[0] <= 1) && (p[1] >= -1) && (p[1] <= 1);
	if(inside && consider_z_planes) inside = (p[2] >= -1) && (p[2] <= 1);
	return inside;
}


void camera::set_pose(const pose& ps) {
	pose_ = ps;
	compute_transformations_();
}


void camera::set_field_of_view(angle fov_x, angle fov_y) {
	fox_x_ = fov_x;
	fox_y_ = fov_y;
	compute_transformations_();
}



spherical_coordinates camera::to_spherical(const Eigen::Vector3f& wp) const {
	return spherical_coordinates::from_cartesian(view_ * wp);
}


camera::projected_coordinates camera::to_projected(const Eigen::Vector3f& wp) const {
	Eigen::Vector4f p = view_projection_ * wp.homogeneous();
	p /= p[3];
	return p.head(3);
}


Eigen::Vector3f camera::point(const spherical_coordinates& s) const {
	return view_inv_ * s.to_cartesian();
}


Eigen::Vector3f camera::point(const projected_coordinates& pr) const {
	Eigen::Vector4f p = view_projection_inv_ * pr.homogeneous();
	p /= p[3];
	return p.head(3);
}


}

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
	if(compute) compute_transformations_();
}


void camera::compute_transformations_() {
	view_ = pose_.view_transformation();
	view_inv_ = view_.inverse();

	float dz = far_z_ - near_z_;	
	float tanx = std::tan(fov_x_ / 2);
	float tany = std::tan(fov_x_ / 2);

	Eigen::Matrix4f mat;
	mat <<
		tanx, 0, 0, 0,
		0, tany, 0, 0,
		0, 0, -(far_z_ + near_z_)/dz, (-2.0f*near_z_*far_z_)/dz,
		0, 0, -1, 0;
				
	view_projection_ = Eigen::Projective3f(mat) * view_;
	view_projection_inv_ = view_projection_.inverse();
}


frustum camera::viewing_frustum() const {
	frustum fr( view_projection_.matrix().transpose() );
	return fr;
}


std::array<Eigen::Vector3f, 8> camera::frustum_corners() const {
	float nx = std::tan(fov_x_ / 2) * near_z_;
	float ny = std::tan(fov_x_ / 2) * near_z_;
	float fx = std::tan(fov_x_ / 2) * far_z_;
	float fy = std::tan(fov_x_ / 2) * far_z_;
	std::array<Eigen::Vector3f, 8> corn {
		// Near
		Eigen::Vector3f(-nx, -ny, near_z_),
		Eigen::Vector3f(nx, -ny, near_z_),
		Eigen::Vector3f(nx, ny, near_z_),
		Eigen::Vector3f(-nx, ny, near_z_),
		
		// Far
		Eigen::Vector3f(-fx, -fy, far_z_),
		Eigen::Vector3f(fx, -fy, far_z_),
		Eigen::Vector3f(fx, fy, far_z_),
		Eigen::Vector3f(-fx, fy, far_z_)
	};
	for(Eigen::Vector3f& c : corn) c = view_inv_ * c;
	return corn;
}


std::array<camera::frustum_edge, 12> camera::frustum_edges() const {
	auto corn = frustum_corners();
	return {
		// Near Plane
		frustum_edge(corn[0], corn[1]),
		frustum_edge(corn[1], corn[2]),
		frustum_edge(corn[2], corn[3]),
		frustum_edge(corn[3], corn[0]),
		
		// Far Plane
		frustum_edge(corn[0+4], corn[1+4]),
		frustum_edge(corn[1+4], corn[2+4]),
		frustum_edge(corn[2+4], corn[3+4]),
		frustum_edge(corn[3+4], corn[0+4]),
		
		// Connecting
		frustum_edge(corn[0], corn[0+4]),
		frustum_edge(corn[1], corn[1+4]),
		frustum_edge(corn[2], corn[2+4]),
		frustum_edge(corn[3], corn[3+4])
	};
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
	fov_x_ = fov_x;
	fov_y_ = fov_y;
	compute_transformations_();
}



spherical_coordinates camera::to_spherical(const Eigen::Vector3f& wp) const {
	return spherical_coordinates::from_cartesian(view_ * wp);
}


Eigen::Vector3f camera::point(const spherical_coordinates& s) const {
	return view_inv_ * s.to_cartesian();
}


}

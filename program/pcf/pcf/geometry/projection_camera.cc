#include "projection_camera.h"
#include "angle.h"
#include <iostream>

namespace pcf {

angle projection_camera::compute_fov_y_(angle fvx, image_size sz) {
	float x_pr = std::tan(fvx / 2.0f);
	float y_pr = (float(x_pr) * sz[1]) / sz[0];
	return std::atan(y_pr / 2.0f);
}


angle projection_camera::compute_fov_x_(angle fvy, image_size sz) {
	float y_pr = std::tan(fvy / 2.0f);
	float x_pr = (float(y_pr) * sz[0]) / sz[1];
	return std::atan(x_pr / 2.0f);
}


projection_camera::projection_camera(const pose& ps, angle fvx, std::size_t imw, std::size_t imh, float znear, float zfar) :
camera(ps, fvx, compute_fov_y_(fvx, image_size({imw, imh})), znear, zfar, false),
image_size_({ imw, imh }),
image_center_({ std::ptrdiff_t(imw)/2, std::ptrdiff_t(imh)/2 }) {
	compute_transformations_using_aspect_ratio_();
}


void projection_camera::compute_transformations_using_aspect_ratio_() {
	view_ = pose_.view_transformation();
	view_inv_ = view_.inverse();

	float x_scale = 1.0f / std::tan(fov_x_ / 2);
	float y_scale = (x_scale * image_size_[1]) / image_size_[0];
	float zdiff = far_z_ - near_z_;
	
	Eigen::Matrix4f mat;
	mat <<
		x_scale, 0, 0, 0,
		0, y_scale, 0, 0,
		0, 0, -(far_z_ + near_z_)/zdiff, -1,
		0, 0, -2*near_z_*far_z_/zdiff, 0;
		
	view_projection_ = Eigen::Projective3f(mat) * view_;
	view_projection_inv_ = view_projection_.inverse();
	
	std::cout << view_projection_.matrix() << std::endl;
}

void projection_camera::set_pose(const pose& ps) {
	pose_ = ps;
	compute_transformations_using_aspect_ratio_();
}

float projection_camera::aspect_ratio() const {
	return float(image_size_[0]) / image_size_[1];
}


projection_camera::image_size projection_camera::get_image_size() const {
	return image_size_;
}

void projection_camera::set_image_size(std::size_t imw, std::size_t imh) {
	image_size_ = image_size { imw, imh };
	image_center_ = image_coordinates { std::ptrdiff_t(imw)/2, std::ptrdiff_t(imh)/2 };
	compute_transformations_using_aspect_ratio_();
}


projection_camera::image_coordinates projection_camera::to_projected(const Eigen::Vector3f& p) const {
	Eigen::Vector4f projected_coordinates = view_projection_ * p.homogeneous();
	projected_coordinates /= projected_coordinates[3];
	
	for(std::ptrdiff_t i = 0; i < 2; ++i) {
		projected_coordinates[i] *= 0.5f * image_size_[i];
		projected_coordinates[i] += image_center_[i];
	}
	
	return image_coordinates {
		(std::ptrdiff_t)projected_coordinates[0],
		(std::ptrdiff_t)projected_coordinates[1]
	};
}


void projection_camera::set_field_of_view_x(angle fvx) {
	fov_x_ = fvx;
	fov_y_ = compute_fov_y_(fvx, image_size_);
}


void projection_camera::set_field_of_view_y(angle fvy) {
	fov_x_ = compute_fov_x_(fvy, image_size_);
	fov_y_ = fvy;
}



void projection_camera::adjust_field_of_view(bool keep_x) {
	if(keep_x) fov_y_ = compute_fov_y_(fov_x_, image_size_);
	else fov_x_ = compute_fov_x_(fov_y_, image_size_);
}


void projection_camera::adjust_image_size(bool keep_width) {
	/// TODO
}


spherical_coordinates projection_camera::projected_to_spherical(const image_coordinates& ic, float distance) const {
	 Eigen::Vector3f world_coordinates = point(ic, 1);
	 spherical_coordinates s = spherical_coordinates::from_cartesian(world_coordinates);
	 s.radius = distance;
	 return s;
}


Eigen::Vector3f projection_camera::point(const image_coordinates& ic, float z) const {
	Eigen::Vector3f projected_coordinates(0, 0, z);
	
	for(std::ptrdiff_t i = 0; i < 2; ++i) {
		projected_coordinates[i] = ic[i];
		projected_coordinates[i] -= image_center_[i];
		projected_coordinates[i] /= 0.5f * image_size_[i];
	}

	Eigen::Vector4f world_coordinates = view_projection_inv_ * projected_coordinates.homogeneous();
	world_coordinates /= world_coordinates[3];
	return world_coordinates.head(3);	
}



}

#include "camera.h"

namespace pcf {

camera::camera(pose p, const Eigen::Projective3f& proj, std::size_t iw, std::size_t ih) :
pose_(p), projection_(proj), image_width_(iw), image_height_(ih) {
	update_();
}

void camera::set_pose(const pose& p) {
	pose_ = p;
	update_();
}

void camera::set_projection(const Eigen::Projective3f& proj) {
	projection_ = proj;
	update_();
}

void camera::set_image_size(std::size_t w, std::size_t h) {
	image_width_ = w;
	image_height_ = h;
	update_();
}

void camera::update_() {
	view_ = pose_.view_transformation();
	view_projection_ = projection_ * view_;
	image_center_ = { std::ptrdiff_t(image_width_ / 2), std::ptrdiff_t(image_height_ / 2) };
}


camera::image_coordinates camera::project(const Eigen::Vector3f& pt) const {
	Eigen::Vector3f ipt = view_projection_ * pt;
	return {
		image_center_[0] + std::ptrdiff_t(ipt[0] * image_width_ / 2.0f),
		image_center_[1] + std::ptrdiff_t(ipt[1] * image_height_ / 2.0f)
	};
}


bool camera::in_frustum(const Eigen::Vector3f& pt) const {
	Eigen::Vector3f ipt = view_projection_ * pt;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		if(ipt[i] < -1 || ipt[i] > 1) return false;
	}
	return true;
}


float camera::range(const Eigen::Vector3f& pt) const {
	Eigen::Vector3f ipt = view_projection_ * pt;
	return ipt[2];
}

float camera::depth(const Eigen::Vector3f& pt) const {
	Eigen::Vector3f ipt = view_ * pt;
	return ipt.norm();
}


}
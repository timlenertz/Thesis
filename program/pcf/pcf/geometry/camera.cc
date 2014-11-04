#include "camera.h"
#include "../geometry/projection.h"

namespace pcf {

camera::camera(const pose& p, float fov, float znear, float zfar, std::size_t iw, std::size_t ih) :
pose_(p), projection_( perspective_projection(float(iw)/ih, fov, znear, zfar) ), image_width_(iw), image_height_(ih) {
	update_();
}

camera::camera(const pose& p, const Eigen::Projective3f& proj, std::size_t iw, std::size_t ih) :
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


void camera::set_perspective_projection(float fov, float znear, float zfar) {
	set_projection( perspective_projection(aspect_ratio(), fov, znear, zfar) );
}


void camera::set_image_size(std::size_t w, std::size_t h) {
	image_width_ = w;
	image_height_ = h;
	update_();
}

float camera::aspect_ratio() const {
	return float(image_width_) / image_height_;
}

void camera::update_() {
	view_ = pose_.view_transformation();
	view_projection_ = projection_ * view_;
	image_center_ = { std::ptrdiff_t(image_width_ / 2), std::ptrdiff_t(image_height_ / 2) };
}


camera::image_coordinates camera::project(const Eigen::Vector3f& pt) const {
	Eigen::Vector4f ipt = view_projection_ * pt.homogeneous();
	ipt /= ipt[3];
	return {
		image_center_[0] + std::ptrdiff_t(ipt[0] * image_width_ / 2.0f),
		image_center_[1] + std::ptrdiff_t(ipt[1] * image_height_ / 2.0f)
	};
}


Eigen::Vector3f camera::reverse_project_with_depth(image_coordinates c, float depth) const {
	Eigen::Vector3f ic;
	ic[0] = float(c[0] - image_center_[0]) / (2.0f * image_width_);
	ic[1] = float(c[1] - image_center_[1]) / (2.0f * image_height_);
	ic[2] = std::sqrt(depth*depth - ic[0]*ic[0] - ic[1]*ic[1]);
	
	Eigen::Vector4f oc = view_projection_.inverse() * ic.homogeneous();
	oc /= oc[3];
	
	return oc.head(3);
}


bool camera::in_frustum(const Eigen::Vector3f& pt) const {
	Eigen::Vector4f ipt = view_projection_ * pt.homogeneous();
	ipt /= ipt[3];
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		if(ipt[i] < -1 || ipt[i] > 1) return false;
	}
	return true;
}


float camera::range(const Eigen::Vector3f& pt) const {
	Eigen::Vector4f ipt = view_projection_ * pt.homogeneous();
	return ipt[2] / ipt[3];
}

float camera::depth(const Eigen::Vector3f& pt) const {
	Eigen::Vector3f vpt = view_ * pt;
	return vpt.norm();
}


}
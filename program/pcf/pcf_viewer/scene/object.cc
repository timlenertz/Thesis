#include "object.h"
#include "scene.h"

namespace pcf {

scene_object::scene_object(const scene& sc, const pose& ps) :
space_object(ps), scene_(sc) {
	compute_mvp_matrix_();
}

void scene_object::compute_mvp_matrix_() {
	const projection_image_camera& cam = scene_.get_camera();
	mvp_matrix_ = cam.view_projection_transformation().matrix() * absolute_pose().view_transformation_inverse().matrix();
}


void scene_object::pose_or_camera_was_updated_() {
	// Can be overloaded by subclass
	return;
}


void scene_object::pose_was_updated_() {
	compute_mvp_matrix_();
	this->pose_or_camera_was_updated_();
}


void scene_object::updated_camera() {
	compute_mvp_matrix_();
	this->pose_or_camera_was_updated_();
}


}
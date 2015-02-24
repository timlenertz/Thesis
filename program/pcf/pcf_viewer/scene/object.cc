#include "object.h"
#include "scene.h"

namespace pcf {

scene_object::scene_object(const scene& sc, const space_object& obj) :
scene_(sc), object_(obj) {
	compute_mvp_matrix_();
}


void scene_object::compute_mvp_matrix_() {
	const projection_image_camera& cam = scene_.get_camera();
	mvp_matrix_ = cam.view_projection_transformation().matrix() * object_.absolute_pose().view_transformation_inverse().matrix();
}


void scene_object::mvp_was_updated_() {
	return;
}


void scene_object::handle_camera_update() {
	compute_mvp_matrix_();
	this->mvp_was_updated_();
}


void scene_object::handle_pose_update() {
	compute_mvp_matrix_();
	this->mvp_was_updated_();
}



}
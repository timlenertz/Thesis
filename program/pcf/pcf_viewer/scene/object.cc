#include "object.h"
#include "scene.h"

namespace pcf {

scene_object::scene_object(const scene& sc) :
scene_(sc),
model_transformation_(Eigen::Affine3f::Identity()) {
	compute_mvp_matrix_();
}


void scene_object::compute_mvp_matrix_() {
	mvp_matrix_ =
		scene_.camera().view_projection_transformation().matrix() *
		model_transformation_.matrix();
}


void scene_object::mvp_was_updated_() {
	return;
}


void scene_object::handle_camera_update() {
	compute_mvp_matrix_();
	this->mvp_was_updated_();
}


void scene_object::set_model_transformation(const Eigen::Affine3f& t) {
	model_transformation_ = t;
	compute_mvp_matrix_();
	this->mvp_was_updated_();
}


bool scene_object::visible() const {
	return should_draw_;
}

void scene_object::set_visible(bool v) {
	should_draw_ = v;
}




}
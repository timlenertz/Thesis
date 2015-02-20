#include "object.h"
#include "scene.h"

namespace pcf {

scene_object::scene_object(const scene& sc, space_object& obj) :
space_object_observer(obj),
scene_(sc), object_(obj) {
	compute_mvp_matrix_();
}

void scene_object::compute_mvp_matrix_() {
	const projection_image_camera& cam = scene_.get_camera();
	mvp_matrix_ = cam.view_projection_transformation().matrix() * object_.absolute_pose().view_transformation_inverse().matrix();
}


void scene_object::pose_was_updated_() {
	compute_mvp_matrix_();
	this->mvp_was_updated_();
}

void scene_object::object_was_deleted_() {
	return;
}

void scene_object::mvp_was_updated_() {
	return;
}


void scene_object::handle_camera_update() {
	compute_mvp_matrix_();
	this->mvp_was_updated_();
}


scene_space_object::scene_space_object(const scene& sc, const pose& ps) :
	space_object(ps), scene_object(sc, *this) { }

scene_space_object::scene_space_object(const scene& sc, const pose& ps, space_object& parent) :
	space_object(ps, parent), scene_object(sc, *this) { }



}
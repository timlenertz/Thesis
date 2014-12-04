#include "object.h"
#include "scene.h"

namespace pcf {

scene_object::scene_object(const scene& sc, const pose& ps) :
scene_(sc), pose_(ps) {
	compute_mvp_matrix_();
}


void scene_object::compute_mvp_matrix_() {
	const auto& cam = scene_.get_camera();
	mvp_matrix_ = cam.view_projection_transformation().matrix() * pose_.view_transformation_inverse().matrix();
}


void scene_object::updated_camera_or_pose_() {
	// Can be overloaded by subclass
}


void scene_object::updated_camera_or_pose() {
	compute_mvp_matrix_();
	this->updated_camera_or_pose_();
}


const pose& scene_object::get_pose() {
	return pose_;
}


void scene_object::set_pose(const pose& ps) {
	pose_ = ps;
	compute_mvp_matrix_();
	this->updated_camera_or_pose();
}


}
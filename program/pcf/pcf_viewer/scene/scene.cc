#include "scene.h"
#include "object.h"
#include "shader_program.h"
#include "bounding_box.h"
#include "point_cloud.h"
#include "../../pcf_core/space_object_wrapper.h"


namespace pcf {

namespace {
	pose initial_camera_pose_ = pose();
}

scene::scene(std::size_t view_w, std::size_t view_h, angle fov_x) :
space_object_observer(),
camera_(
	initial_camera_pose_,
	projection_frustum::symmetric_perspective_fov_x(fov_x, float(view_w)/view_h),
	view_w,
	view_h
) {
	// Observe own camera...
	space_object_observer::set_observed_object(camera_);
}	



void scene::notify_camera_update_() {
	for(auto& obj : objects_) obj->handle_camera_update();
}

scene::~scene() {
	space_object_observer::set_no_observed_object();
}


void scene::pose_was_updated_() {
	// Called when camera pose was changed
	notify_camera_update_();
}


void scene::object_was_updated_() {
	// Called when camera parameters were changed
	notify_camera_update_();
}


void scene::clear() {
	objects_.clear();
	holders_.clear();
}


void scene::add_object_(scene_object& sobj){
	objects_.insert(&sobj);
}


void scene::erase_object_(scene_object& sobj) {
	objects_.erase(&sobj);
}


scene_point_cloud& scene::add(point_cloud_xyz& pc) {
	return add_with_holder_<scene_point_cloud>(pc);
}


scene_point_cloud& scene::add(point_cloud_full& pc) {
	return add_with_holder_<scene_point_cloud>(pc);
}

scene_bounding_box& scene::add(space_bounding_box& box) {
	return add_with_holder_<scene_bounding_box>(box);
}


const scene_object_holder_base& scene::holder_with_name_(const std::string& nm) const {
	for(auto& h : holders_)
		if(h->get_space_object().get_name() == nm) return *h;
	throw std::invalid_argument("No object with that name.");
}

scene_object_holder_base& scene::holder_with_name_(const std::string& nm) {
	for(const auto& h : holders_)
		if(h->get_space_object().get_name() == nm) return *h;
	throw std::invalid_argument("No object with that name.");
}

bool scene::has_holder_with_name_(const std::string& nm) const {
	for(const auto& h : holders_)
		if(h->get_space_object().get_name() == nm) return true;
	return false;
}


scene_object& scene::scene_object_with_name(const std::string& nm) {
	return holder_with_name_(nm).get_scene_object();
}


const scene_object& scene::scene_object_with_name(const std::string& nm) const {
	return holder_with_name_(nm).get_scene_object();
}


space_object& scene::space_object_with_name(const std::string& nm) {
	return holder_with_name_(nm).get_space_object();
}


const space_object& scene::space_object_with_name(const std::string& nm) const {
	return holder_with_name_(nm).get_space_object();
}


bool scene::has_object_with_name(const std::string& nm) const {
	return has_holder_with_name_(nm);
}


const rgb_color& scene::get_background_color() const {
	return background_color_;
}


void scene::set_background_color(const rgb_color& col) {
	background_color_ = col;
	need_reset_ = true;
}



void scene::gl_initialize_() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	for(auto& obj : objects_)
		if(! obj->initialized()) obj->initialize();
}


void scene::gl_draw_() {
	if(need_reset_) {
		glViewport(0, 0, camera_.image_width(), camera_.image_height());

		auto bg = background_color_.to_float();
		glClearColor(bg[0], bg[1], bg[2], 1.0f);
		
		need_reset_ = false;
	}
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(auto& obj : objects_)
		obj->draw();    
}


}
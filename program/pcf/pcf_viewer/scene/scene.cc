#include "scene.h"
#include "object.h"
#include "shader_program.h"
#include "bounding_box.h"
#include "frustum.h"


namespace pcf {

namespace {
	pose initial_pose_ = pose();
}

scene::scene(std::size_t view_w, std::size_t view_h, angle fov_x) :
	camera_(
		initial_pose_,
		projection_frustum::symmetric_perspective_fov_x(fov_x, float(view_w)/view_h),
		view_w,
		view_h
	) { }	


void scene::notify_camera_update_() {
	for(auto& obj : objects_) obj->handle_camera_update();
}

scene::~scene() { }


const projection_image_camera& scene::get_camera() const {
	return camera_;
}


void scene::set_camera(const projection_image_camera& cam) {
	camera_ = cam;
	notify_camera_update_();
}


void scene::set_camera_pose(const pose& ps) {
	camera_.set_pose(ps);
	notify_camera_update_();
}


const pose& scene::get_camera_pose() const {
	return camera_.get_pose();
}


void scene::set_camera_image_size(std::size_t w, std::size_t h) {
	camera_.set_image_size(w, h);
	camera_.adjust_field_of_view_y();
	should_reset_viewport_ = true;
	notify_camera_update_();
}


void scene::clear() {
	objects_.clear();
}


void scene::add_object(scene_object& sobj){
	objects_.emplace_back(&sobj);
}


void scene::erase_object(scene_object& sobj) {
	objects_.erase(&sobj);
}



void scene::gl_initialize_() {
	glEnable(GL_DEPTH_TEST);
	
	auto bg = background_color_.to_float();
	glClearColor(bg[0], bg[1], bg[2], 1.0f);

	for(auto& obj : objects_)
		if(! obj->initialized()) obj->initialize();
}


void scene::gl_draw_() {
	if(should_reset_viewport_) {
		glViewport(0, 0, camera_.image_width(), camera_.image_height());
		should_reset_viewport_ = false;
	}
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(auto& obj : objects_) obj->draw();    
}


}
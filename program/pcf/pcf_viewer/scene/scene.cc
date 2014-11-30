#include "scene.h"
#include "object.h"
#include "shader_program.h"
#include "bounding_box.h"


namespace pcf {


scene::scene(std::size_t view_w, std::size_t view_h, angle fov_x) :
	camera_(pose(), fov_x, view_w, view_h) { }	


scene::~scene() { }


const projection_camera& scene::get_camera() const {
	return camera_;
}


void scene::set_camera(const projection_camera& cam) {
	camera_ = cam;
	update_camera(camera_);
}


void scene::set_camera_pose(const pose& ps) {
	camera_.set_pose(ps);
	update_camera(camera_);
}


const pose& scene::get_camera_pose() const {
	return camera_.get_pose();
}


void scene::set_camera_image_size(std::size_t w, std::size_t h) {
	camera_.set_image_size(w, h);
	camera_.adjust_field_of_view();
	should_reset_viewport_ = true;
	update_camera(camera_);
}

void scene::set_camera_field_of_view(angle fov_x) {
	camera_.set_field_of_view_x(fov_x);
	update_camera(camera_);
}


scene_bounding_box& scene::add_bounding_box(const bounding_box& box) {
	scene_bounding_box* sobj = new scene_bounding_box(*this, box);
	objects_.emplace(sobj);
	return *sobj;
}


void scene::gl_initialize_() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	for(const auto& obj : objects_) if(! obj->initialized()) obj->initialize();
}


void scene::update_camera(const projection_camera& cam) {
	for(const auto& obj : objects_) obj->update_camera(cam);
}


void scene::gl_draw_() {
	if(should_reset_viewport_) {
		glViewport(0, 0, camera_.image_width(), camera_.image_height());
		should_reset_viewport_ = false;
	}
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(const auto& obj : objects_) obj->draw();    
}


}
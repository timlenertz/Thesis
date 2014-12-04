#include "scene.h"
#include "object.h"
#include "shader_program.h"
#include "bounding_box.h"
#include "frustum.h"


namespace pcf {


scene::scene(std::size_t view_w, std::size_t view_h, angle fov_x) :
	camera_(pose(), fov_x, view_w, view_h) { }	


scene::~scene() { }


const projection_camera& scene::get_camera() const {
	return camera_;
}


void scene::set_camera(const projection_camera& cam) {
	camera_ = cam;
	updated_camera();
}


void scene::set_camera_pose(const pose& ps) {
	camera_.set_pose(ps);
	updated_camera();
}


const pose& scene::get_camera_pose() const {
	return camera_.get_pose();
}


void scene::set_camera_image_size(std::size_t w, std::size_t h) {
	camera_.set_image_size(w, h);
	camera_.adjust_field_of_view();
	should_reset_viewport_ = true;
	updated_camera();
}

void scene::set_camera_field_of_view(angle fov_x) {
	camera_.set_field_of_view_x(fov_x);
	updated_camera();
}


scene_bounding_box& scene::add_bounding_box(const bounding_box& box) {
	scene_bounding_box* sobj = new scene_bounding_box(*this, box);
	objects_.emplace(sobj);
	return *sobj;
}

scene_frustum& scene::add_frustum(const frustum& fr) {
	scene_frustum* sobj = new scene_frustum(*this, fr);
	objects_.emplace(sobj);
	return *sobj;
}

void scene::clear() {
	objects_.clear();
}


void scene::gl_initialize_() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	for(const auto& obj : objects_) if(! obj->initialized()) obj->initialize();
}


void scene::updated_camera() {
	for(const auto& obj : objects_) obj->updated_camera_or_pose();
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
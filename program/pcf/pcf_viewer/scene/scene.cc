#include "scene.h"
#include "scene_object.h"
#include "scene_object_shader_program.h"

namespace pcf {



scene::scene(std::size_t view_w, std::size_t view_h, angle fov_x) :
	camera_(pose(), fov_x, fov_y_(fov_x, view_w, view_h)) { }	


scene::~scene() { }
	
void scene::set_camera_parameters(std::size_t view_w, std::size_t view_h, angle fov_x) {
	camera_.set_field_of_view(fov_x, fov_y_(fov_x, view_w, view_h));
	update_camera(camera_);
}

void scene::set_camera_pose(const pose& ps) {
	camera_.set_pose(ps);
	update_camera(camera_);
}


const pose& get_camera_pose() const {
	return camera_.get_pose();
}


void scene::gl_initialize_() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	for(const auto& obj : objects_) if(! obj->initialized()) obj->initialize();
}


void scene::update_camera(const camera& cam) {
	for(const auto& obj : objects_) obj->update_camera(cam);
}


void scene::gl_draw_() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(const auto& obj : objects_) obj->draw();
}


}
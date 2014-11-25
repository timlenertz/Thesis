#include "scene.h"
#include "scene_object.h"
#include "scene_object_shader_program.h"

namespace pcf {

scene::~scene() { }

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
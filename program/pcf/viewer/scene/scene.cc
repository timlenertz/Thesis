#include "scene.h"
#include "scene_object.h"

namespace pcf {

scene::~scene() { }


void scene::gl_draw_() {
	for(const auto& obj : objects_) obj->draw();
}


}
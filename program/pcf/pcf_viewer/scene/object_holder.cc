#include "object_holder.h"
#include "scene.h"

namespace pcf {

// Resolves cross-reference between scene.h and object_holder.h

void scene_object_holder_base::add_to_scene_(scene_object& sobj) {
	scene_.add_object(sobj);
}


void scene_object_holder_base::erase_from_scene_(scene_object& sobj) {
	scene_.erase_object(sobj);
}


}
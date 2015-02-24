#include <utility>
#include <stdexcept>
#include "scene.h"

namespace pcf {


template<typename Scene_object, typename... Construction_args>
Scene_object* scene_object_holder<Scene_object, Construction_args...>::
create_scene_object_with_args_(Construction_args&... args) const {
	return new Scene_object(observed_object(), args...);
}


template<typename Scene_object, typename... Construction_args>
void scene_object_holder<Scene_object, Construction_args...>::pose_was_updated_() {
	scene_object_.handle_pose_update();
}


template<typename Scene_object, typename... Construction_args>
void scene_object_holder<Scene_object, Construction_args...>::object_was_updated_() {
	scene_object_.reset(); // Will call object_was_deleted_()
	
	Scene_object* obj = create_scene_object_();
	scene_object_.reset(obj);
	scene_.add_object(obj);
}


template<typename Scene_object, typename... Construction_args>
void scene_object_holder<Scene_object, Construction_args...>::object_was_deleted_() {
	scene_.erase_object(scene_object_);
}


template<typename Scene_object, typename... Construction_args>
scene_object_holder<Scene_object, Construction_args...>::scene_object_holder
(scene& sc, Construction_args&&... args) :
	space_object_observer(obj),
	scene_(sc) :
	create_scene_object_(std::bind(
		&scene_object_holder::create_scene_object_with_args_,
		sc, args...
	))
{ 
	Scene_object* obj = create_scene_object_();
	scene_object_.reset(obj);
	scene_.add_object(obj);
}


template<typename Scene_object, typename... Construction_args>
scene_object_holder<Scene_object, Construction_args...>::~scene_object_holder() {
	scene_.erase_object(scene_object_.get());
}


}

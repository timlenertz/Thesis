#include <utility>
#include <stdexcept>
#include <cassert>

#include <iostream>
#include <typeinfo>

#include "object.h"

namespace pcf {


template<typename Scene_object, typename Space_object, typename... Construction_args>
void scene_object_holder<Scene_object, Space_object, Construction_args...>::pose_was_updated_() {
	const pose& p = get_space_object().absolute_pose();
	scene_object_->set_model_transformation(p.transformation_to_world());
}


template<typename Scene_object, typename Space_object, typename... Construction_args>
void scene_object_holder<Scene_object, Space_object, Construction_args...>::object_was_updated_() {
	scene_object* old_obj = scene_object_.release();
	erase_from_scene_(*old_obj);
		
	Scene_object* new_obj = create_scene_object_();
	new_obj->take_parameters(*(Scene_object*)old_obj);
	delete old_obj;

	scene_object_.reset(new_obj);
	pose_was_updated_();
	add_to_scene_(*new_obj);
}


template<typename Scene_object, typename Space_object, typename... Construction_args>
void scene_object_holder<Scene_object, Space_object, Construction_args...>::object_was_deleted_() {
	if(scene_object_) erase_from_scene_(*scene_object_);
}


template<typename Scene_object, typename Space_object, typename... Construction_args>
scene_object_holder<Scene_object, Space_object, Construction_args...>::scene_object_holder
(scene& sc, Space_object& obj, Construction_args... args) :
	scene_object_holder_base(sc, obj)
{
	create_scene_object_ = std::bind(
		&scene_object_holder_base::create_with_args_<Scene_object, scene&, const Space_object&, Construction_args...>,
		std::ref(sc), std::cref(obj), std::ref(args)...
	);

	Scene_object* sobj = create_scene_object_();
	scene_object_.reset(sobj);
	pose_was_updated_();
	add_to_scene_(*sobj);
}


template<typename Scene_object, typename Space_object, typename... Construction_args>
scene_object_holder<Scene_object, Space_object, Construction_args...>::~scene_object_holder() {
	erase_from_scene_(*scene_object_);
}


}

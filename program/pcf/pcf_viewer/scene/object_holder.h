#ifndef PCF_SCENE_OBJECT_HOLDER_H_
#define PCF_SCENE_OBJECT_HOLDER_H_

#include <functional>
#include <memory>
#include "../../pcf_core/space_object_observer.h"

namespace pcf {

class scene;
class space_object;
class scene_object;


/**
Polymorphic base class for scene object holder.
Internal usage.
*/
class scene_object_holder_base : public space_object_observer {
protected:
	scene& scene_;

	scene_object_holder_base(scene& sc, space_object& obj) :
		space_object_observer(obj), scene_(sc) { }
	
	void add_to_scene_(scene_object&);
	void erase_from_scene_(scene_object&);
	
	template<typename Object, typename... Args>
	static Object* create_with_args_(Args&... args) {
		return new Object(args...);
	}

public:
	virtual ~scene_object_holder_base() { }
	
	const space_object& get_space_object() const { return observed_object(); }
	space_object& get_space_object() { return observed_object(); }
	
	virtual const scene_object& get_scene_object() const = 0;
	virtual scene_object& get_scene_object() = 0;
};


/**
Manager class for scene object representing a space object.
Creates the scene object from the space object and adds/erases it from scene. Tracks when space object gets deleted, updated (space_object::handle_update() call) or its pose gets changed. When it got updated, recreates a new scene object for it and replaces it in the scene. Stores the constructor parameters using which to create the scene object. The scene objects themselves are immutable, they don't need to handle change in the space object. Scene object constructor needs to take scene, the space object references as two first parameters.
*/
template<typename Scene_object, typename Space_object, typename... Construction_args>
class scene_object_holder : public scene_object_holder_base {
private:
	std::unique_ptr<Scene_object> scene_object_;
	std::function<Scene_object*()> create_scene_object_; // std::bind object, contains the constructor parameters
	
	void pose_was_updated_() final override;
	void object_was_updated_() final override;
	void object_was_deleted_() final override;

public:
	scene_object_holder(scene&, Space_object&, Construction_args...);
	~scene_object_holder();
	
	const Scene_object& get_scene_object() const { return *scene_object_; }
	Scene_object& get_scene_object() { return *scene_object_; }
};

}

#include "object_holder.tcc"

#endif

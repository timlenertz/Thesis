#ifndef PCF_SCENE_OBJECT_HOLDER_H_
#define PCF_SCENE_OBJECT_HOLDER_H_

#include <functional>
#include <memory>
#include "../../pcf_core/space_object_observer.h"

namespace pcf {

class scene;
class space_object;
class scene_object;

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
};



template<typename Scene_object, typename Space_object, typename... Construction_args>
class scene_object_holder : public scene_object_holder_base {
private:
	std::unique_ptr<Scene_object> scene_object_;
	std::function<Scene_object*()> create_scene_object_;
	
	void pose_was_updated_() final override;
	void object_was_updated_() final override;
	void object_was_deleted_() final override;

public:
	scene_object_holder(scene&, Space_object&, Construction_args...);
	~scene_object_holder();
};

}

#include "object_holder.tcc"

#endif

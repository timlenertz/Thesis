#ifndef PCF_SCENE_OBJECT_HOLDER_H_
#define PCF_SCENE_OBJECT_HOLDER_H_

#include <functional>
#include <memory>
#include "../../pcf_core/space_object_observer.h"

namespace pcf {

class scene;
class space_object;

template<typename Scene_object, typename... Construction_args>
class scene_object_holder : public space_object_observer {
private:
	std::unique_ptr<Scene_object> scene_object_;
	scene& scene_;
	const std::function<Scene_object*()> create_scene_object_;
	
	Scene_object* create_scene_object_with_args_(Construction_args&...) const;
	
	void pose_was_updated_() final override;
	void object_was_updated_() final override;
	void object_was_deleted_() final override;

public:
	scene_object_holder(scene&, Construction_args&&...);
	~scene_object_holder();
};

}

#include "object_holder.tcc"

#endif

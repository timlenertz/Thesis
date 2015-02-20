#ifndef PCFVW_SCENE_OBJECT_H_
#define PCFVW_SCENE_OBJECT_H_

#include "../gl_object.h"
#include "../../pcf_core/geometry/pose.h"
#include "../../pcf_core/space_object.h"

namespace pcf {

class projection_camera;
class scene;

/**
Base class for embedding of a space object in a scene.
Handles computation of MVP matrix based on object and camera pose. Subclass handles rendering of object.
*/
class scene_object : public gl_object, public space_object_observer {
protected:
	Eigen::Matrix4f mvp_matrix_;
	
	const scene& scene_;
	space_object& object_;
	
	scene_object(const scene&, space_object& obj);		
	scene_object(const scene_object&) = delete;
	scene_object& operator=(const scene_object&) = delete;
			
	void pose_was_updated_() final override;
	void object_was_deleted_() final override;

	virtual void mvp_was_updated_();

private:
	void compute_mvp_matrix_();

public:
	void handle_camera_update();
};


/**
Scene object with internal space object instance.
Used for embedding of primitive object in scene. Adds space object attributes to it.
*/
class scene_space_object : public space_object, public scene_object {
protected:
	scene_space_object(const scene&, const pose& = pose());
	scene_space_object(const scene&, const pose&, space_object& parent);
};

}

#endif
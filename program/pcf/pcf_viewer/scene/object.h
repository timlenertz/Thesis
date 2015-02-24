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
Handles computation of MVP matrix based on object and camera pose. Subclass handles rendering of object. Subclass constructors always take at least 2 arguments: scene, space object (both reference to const), followed by additional parameters. 
*/
class scene_object : public gl_object {
protected:
	const scene& scene_;
	const space_object& object_;
	Eigen::Matrix4f mvp_matrix_;
		
	scene_object(const scene&, const space_object&);
					
	virtual void mvp_was_updated_();

private:
	void compute_mvp_matrix_();

	scene_object(const scene_object&) = delete;

public:
	void handle_camera_update();
	void handle_pose_update();
	
	const scene& get_scene() const { return scene_; }
};

}

#endif
#ifndef PCFVW_SCENE_OBJECT_H_
#define PCFVW_SCENE_OBJECT_H_

#include "../gl_object.h"
#include "../../pcf_core/geometry/pose.h"
#include "../../pcf_core/space_object.h"

namespace pcf {

class projection_camera;
class scene;

class scene_object : public gl_object, public space_object {
protected:
	const scene& scene_;
	
	/// Full MVP matrix.
	/// Includes camera position/parameters and space object pose.
	Eigen::Matrix4f mvp_matrix_;
	
	explicit scene_object(const scene&, const pose& = pose());	
	scene_object(const scene_object&) = delete;
	scene_object& operator=(const scene_object&) = delete;
	
	void compute_mvp_matrix_();
	
	virtual void pose_or_camera_was_updated_();

	void pose_was_updated_() override;
	
public:
	/// Must be called when scene camera was changed.
	/// Can be called while not in OpenGL context.
	void updated_camera();
};

}

#endif
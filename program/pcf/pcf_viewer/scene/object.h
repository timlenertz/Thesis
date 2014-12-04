#ifndef PCFVW_SCENE_OBJECT_H_
#define PCFVW_SCENE_OBJECT_H_

#include "../gl_object.h"
#include "../../pcf/geometry/pose.h"

namespace pcf {

class projection_camera;
class scene;

class scene_object : public gl_object {
protected:
	const scene& scene_;
	pose pose_;
	Eigen::Matrix4f mvp_matrix_;
	
	explicit scene_object(const scene&, const pose& = pose());	
	scene_object(const scene_object&) = delete;
	scene_object& operator=(const scene_object&) = delete;
	
	/**
	Called when scene camera and/or object's pose was changed.
	Can be called while not in OpenGL context.
	*/
	virtual void updated_camera_or_pose_();
	
private:
	void compute_mvp_matrix_();

public:
	void updated_camera_or_pose();
	
	const pose& get_pose();
	void set_pose(const pose&);
};

}

#endif
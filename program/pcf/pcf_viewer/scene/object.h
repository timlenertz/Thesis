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
	
	scene_object(const scene& sc) : scene_(sc) { }
	
	scene_object(const scene_object&) = delete;
	scene_object& operator=(const scene_object&) = delete;

public:
	virtual void update_camera(const projection_camera&) { }
};

}

#endif
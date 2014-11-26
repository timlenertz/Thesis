#ifndef PCFVW_SCENE_OBJECT_H_
#define PCFVW_SCENE_OBJECT_H_

#include "../gl_object.h"

namespace pcf {

class camera;
class scene;

class scene_object : public gl_object {
protected:
	scene& scene_;

	explicit scene_object(scene& sc) : scene_(sc) { }

public:
	virtual void update_camera(const camera&) { }
}

#endif
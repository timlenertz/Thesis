#ifndef PCFVW_SCENE_OBJECT_H_
#define PCFVW_SCENE_OBJECT_H_

#include "../gl_object.h"

namespace pcf {

class camera;

class scene_object : public gl_object { };
public:
	virtual void updated_camera(const camera&) { }
}

#endif
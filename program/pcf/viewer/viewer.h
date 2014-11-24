#ifndef PCFVW_VIEWER_H_
#define PCFVW_VIEWER_H_

#include "gl.h"
#include "scene/scene.h"
#include <pcf/geometry/camera.h>

namespace pcf {

class viewer {
private:
	scene scene_;
	camera camera_;
		
public:
	

	void draw() const;
};

}

#endif
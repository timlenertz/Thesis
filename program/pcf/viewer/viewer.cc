#include "viewer.h"
#include "gl.h"

namespace pcf {

void viewer::initialize_() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
	
void viewer::draw() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	scene_.draw();
}


}
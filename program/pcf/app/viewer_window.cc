#include "viewer_window.h"

namespace pcf {

namespace {
	const char window_title_[] = "PCF Viewer";
	const int default_window_width_ = 800;
	const int default_window_height_ = 600;
}

viewer_window::viewer_window() :
viewer_(default_window_width_, default_window_height_) {
	glfwWindowHint(GLFW_RESIZABLE, 1);

	window_ = glfwCreateWindow(
		default_window_width_,
		default_window_height_,
		window_title_,
		NULL,
		NULL
	);
}


void viewer_window::run() {	
	while(! glfwWindowShouldClose(window_)) {
		if(scene_access_mut_.try_lock()) {	
			glfwMakeContextCurrent(window_);
			viewer_.draw();
			scene_access_mut_.unlock();		
			glfwSwapBuffers(window_);
		}
		
		glfwPollEvents();
	}
}


viewer_window::~viewer_window() {
	glfwDestroyWindow(window_);
}

}
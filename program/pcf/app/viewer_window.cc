#include "viewer_window.h"

namespace pcf {

namespace {
	const char window_title_[] = "PCF Viewer";
	const int default_window_width_ = 800;
	const int default_window_height_ = 600;
}

viewer_window::viewer_window(scene& sc) :
viewer_(default_window_width_, default_window_height_) {
	glfwWindowHint(GLFW_RESIZABLE, 1);

	window_ = glfwCreateWindow(
		default_window_width_,
		default_window_height_,
		window_title_,
		NULL,
		NULL
	);
	
	while(! glfwWindowShouldClose(window_)) {
		glfwMakeContextCurrent(window_);
		
		viewer_.draw();
		glfwSwapBuffers(window_);
	}
}


viewer_window::~viewer_window() {
	glfwDestroyWindow(window_);
}

}
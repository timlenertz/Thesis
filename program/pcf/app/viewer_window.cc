#include "viewer_window.h"
#include "../pcf/geometry/math_constants.h"

namespace pcf {

namespace {
	const char window_title_[] = "PCF Viewer";
	const int default_window_width_ = 800;
	const int default_window_height_ = 600;
}

void viewer_window::window_size_(GLFWwindow* win, int w, int h) {
	viewer_window* that = (viewer_window*) glfwGetWindowUserPointer(win);
	
	that->viewer_.resize_viewport(w, h);
}


void viewer_window::window_cursor_position_(GLFWwindow* win, double x, double y) {
	viewer_window* that = (viewer_window*) glfwGetWindowUserPointer(win);
	
	if( glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ) {
		double factor = pi / 500.0;
	
		double diff_x = x - that->drag_position_x_;
		double diff_y = y - that->drag_position_y_;
		that->drag_position_x_ = x;
		that->drag_position_y_ = y;
		
		that->viewer_.rotate_camera(diff_x * factor, diff_y * factor);
	}
}



void viewer_window::window_mouse_button_(GLFWwindow* win, int button, int action, int mods) {
	viewer_window* that = (viewer_window*) glfwGetWindowUserPointer(win);
	
	glfwGetCursorPos(win, &that->drag_position_x_, &that->drag_position_y_);
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
	
	glfwSetWindowUserPointer(window_, (void*)this);
	
	glfwSetWindowSizeCallback(window_, viewer_window::window_size_);
	glfwSetCursorPosCallback(window_, viewer_window::window_cursor_position_);
	glfwSetMouseButtonCallback(window_, viewer_window::window_mouse_button_);
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
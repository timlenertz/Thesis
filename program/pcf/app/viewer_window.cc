#include "viewer_window.h"
#include "../pcf/geometry/math_constants.h"
#include <Eigen/Eigen>
#include <stdexcept>

namespace pcf {

namespace {
	const char window_title_[] = "PCF Viewer";
	const int default_window_width_ = 800;
	const int default_window_height_ = 600;
	const angle rotation_per_cursor_pixel_ = pi / 500.0;
}


void viewer_window::window_framebuffer_size_(GLFWwindow* win, int w, int h) {
	viewer_window* that = (viewer_window*) glfwGetWindowUserPointer(win);
	
	that->viewer_.resize_viewport(w, h);
}


void viewer_window::window_cursor_position_(GLFWwindow* win, double x, double y) {
	viewer_window* that = (viewer_window*) glfwGetWindowUserPointer(win);
	
	if( glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ) {	
		double diff_x = x - that->drag_position_x_;
		double diff_y = y - that->drag_position_y_;
		that->drag_position_x_ = x;
		that->drag_position_y_ = y;
		
		that->viewer_.rotate_camera(diff_x * rotation_per_cursor_pixel_, diff_y * rotation_per_cursor_pixel_);
	}
}



void viewer_window::window_mouse_button_(GLFWwindow* win, int button, int action, int mods) {
	viewer_window* that = (viewer_window*) glfwGetWindowUserPointer(win);
	
	glfwGetCursorPos(win, &that->drag_position_x_, &that->drag_position_y_);
}


void viewer_window::update_movement_velocity_() {
	Eigen::Vector3f vel;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		switch(movement_directions_[i]) {
			case stop: vel[i] = 0; break;
			case positive: vel[i] = movement_speed_; break;
			case negative: vel[i] = -movement_speed_; break;
		}
	}
	viewer_.set_target_velocity(vel);
}


void viewer_window::window_key_(GLFWwindow* win, int key, int scancode, int action, int mods) {
	viewer_window* that = (viewer_window*) glfwGetWindowUserPointer(win);

	std::ptrdiff_t dim;
	bool pos;
	
	switch(key) {
	case GLFW_KEY_UP:
		dim = 2; pos = false; break;
	case GLFW_KEY_DOWN:
		dim = 2; pos = true; break;
	case GLFW_KEY_LEFT:
		dim = 0; pos = false; break;
	case GLFW_KEY_RIGHT:
		dim = 0; pos = true; break;
	case GLFW_KEY_SPACE:
		dim = 1; pos = true; break;
	case GLFW_KEY_LEFT_SUPER: case GLFW_KEY_LEFT_ALT:
		dim = 1; pos = false; break;
	default:
		return;
	}
	
	if(action == GLFW_PRESS) that->movement_directions_[dim] = (pos ? positive : negative);
	else if(action == GLFW_RELEASE) that->movement_directions_[dim] = stop;
	else return;
	
	that->update_movement_velocity_();
}


viewer_window::viewer_window() :
viewer_(default_window_width_, default_window_height_) {
	for(std::ptrdiff_t i = 0; i < 3; ++i) movement_directions_[i] = stop;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, 1);

	window_ = glfwCreateWindow(
		default_window_width_,
		default_window_height_,
		window_title_,
		NULL,
		NULL
	);
	if(window_ == NULL) throw std::runtime_error("Could not create GLFW window.");
	
	glfwSetWindowUserPointer(window_, (void*)this);
	
	glfwSetFramebufferSizeCallback(window_, &viewer_window::window_framebuffer_size_);
	glfwSetCursorPosCallback(window_, &viewer_window::window_cursor_position_);
	glfwSetMouseButtonCallback(window_, &viewer_window::window_mouse_button_);
	glfwSetKeyCallback(window_, &viewer_window::window_key_);
	
	int w, h;
	glfwGetFramebufferSize(window_, &w, &h);
	viewer_.resize_viewport(w, h);
}


void viewer_window::run() {	
	glfwMakeContextCurrent(window_);
	glfwSwapInterval(100);

	while(! glfwWindowShouldClose(window_)) {
		if(access_viewer_mutex_.try_lock()) {	
			glfwMakeContextCurrent(window_);
			viewer_.draw();
			access_viewer_mutex_.unlock();		
			glfwSwapBuffers(window_);
		}
		
		glfwPollEvents();
	}
	closed_ = true;
}


viewer_window::~viewer_window() {
	if(window_) glfwDestroyWindow(window_);
}

}
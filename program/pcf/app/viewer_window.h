#ifndef PCFAPP_VIEWER_WINDOW_H_
#define PCFAPP_VIEWER_WINDOW_H_

#include "glfw.h"
#include "../pcf_viewer/viewer.h"
#include <mutex>

namespace pcf {

class scene;

class viewer_window {
private:
	GLFWwindow* window_;
	viewer viewer_;
	std::mutex scene_access_mut_;
	double drag_position_x_, drag_position_y_;
	enum { stop, positive, negative } movement_directions_[3];

	void update_movement_velocity_();

	static void window_framebuffer_size_(GLFWwindow* win, int w, int h);
	static void window_cursor_position_(GLFWwindow* win, double x, double y);
	static void window_mouse_button_(GLFWwindow* win, int button, int action, int mods);
	static void window_key_(GLFWwindow* win, int key, int scancode, int action, int mods);

public:
	explicit viewer_window();
	~viewer_window();
	
	template<class Callback>
	void access_scene(Callback cb) {
		scene_access_mut_.lock();
		cb(*viewer_);
		scene_access_mut_.unlock();
	}
	
	void run();
};

}

#endif

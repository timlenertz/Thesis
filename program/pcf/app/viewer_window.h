#ifndef PCFAPP_VIEWER_WINDOW_H_
#define PCFAPP_VIEWER_WINDOW_H_

#include <GLFW/glfw3.h>
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

	static void window_size_(GLFWwindow* win, int w, int h);
	static void window_cursor_position_(GLFWwindow* win, double x, double y);
	static void window_mouse_button_(GLFWwindow* win, int button, int action, int mods);

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

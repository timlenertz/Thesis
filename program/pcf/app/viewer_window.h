#ifndef PCFAPP_VIEWER_WINDOW_H_
#define PCFAPP_VIEWER_WINDOW_H_

#include "glfw.h"
#include <mutex>
#include "../pcf_viewer/viewer.h"

namespace pcf {

class scene;

class viewer_window {
private:
	GLFWwindow* window_;
	viewer viewer_;
	double drag_position_x_, drag_position_y_;
	enum { stop, positive, negative } movement_directions_[3];
	bool closed_ = false;
	float movement_speed_ = 0.05;
	std::mutex access_viewer_mutex_;

	void update_movement_velocity_();

	static void window_framebuffer_size_(GLFWwindow* win, int w, int h);
	static void window_cursor_position_(GLFWwindow* win, double x, double y);
	static void window_mouse_button_(GLFWwindow* win, int button, int action, int mods);
	static void window_key_(GLFWwindow* win, int key, int scancode, int action, int mods);

public:
	explicit viewer_window();
	~viewer_window();
	
	void run();
	bool was_closed() const { return closed_; }
	
	template<typename Callback>
	void access_viewer(const Callback& cb) {
		access_viewer_mutex_.lock();
		cb(viewer_);
		access_viewer_mutex_.unlock();
	}
	
	float get_movement_speed() const { return movement_speed_; }
	void set_movement_speed(float s) { movement_speed_ = s; }
};

}

#endif

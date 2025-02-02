#ifndef PCFAPP_VIEWER_WINDOW_H_
#define PCFAPP_VIEWER_WINDOW_H_

#include "glfw.h"
#include <mutex>
#include <atomic>
#include "../pcf_viewer/viewer.h"

namespace pcf {

class scene;

class viewer_window {
private:
	GLFWwindow* window_;
	viewer viewer_;
	double drag_position_x_, drag_position_y_;
	enum { stop, positive, negative } movement_directions_[3];
	float movement_speed_ = 0.1;
	std::mutex access_viewer_mutex_;
	std::atomic_bool close_signalled_;
	std::atomic_bool close_requested_;
	

	void update_movement_velocity_();

	static void window_framebuffer_size_(GLFWwindow* win, int w, int h);
	static void window_cursor_position_(GLFWwindow* win, double x, double y);
	static void window_mouse_button_(GLFWwindow* win, int button, int action, int mods);
	static void window_key_(GLFWwindow* win, int key, int scancode, int action, int mods);
	static void window_scroll_(GLFWwindow* win, double xoffset, double yoffset);

public:
	explicit viewer_window();
	~viewer_window();
	
	/// Show window with viewer and enter main event loop.
	/// Must be called from main thread. Only exits after signal_close is called (from different thread).
	void run();
	
	/// Request that viewer should be terminated.
	/// Must be called from secondary thread. Asynchronously makes window close, destructs viewer and run function return on main thread.
	void signal_close();
	
	/// Returns true if user has clicked window close button.
	/// Caller should respond by calling signal_close soon after this returns true.
	bool user_requested_close() const;
	
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

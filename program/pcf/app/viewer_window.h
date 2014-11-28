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
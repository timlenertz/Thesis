#ifndef PCFAPP_VIEWER_WINDOW_H_
#define PCFAPP_VIEWER_WINDOW_H_

#include <GLFW/glfw3.h>
#include <pcf_viewer/viewer.h>

namespace pcf {

class scene;

class viewer_window {
private:
	GLFWwindow* window_;
	viewer viewer_;

public:
	explicit viewer_window(scene&);
	~viewer_window();
};

}

#endif
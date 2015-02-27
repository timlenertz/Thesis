#ifndef PCFVW_VIEWER_H_
#define PCFVW_VIEWER_H_

#include "gl.h"
#include "scene/scene.h"
#include "motion_controller.h"
#include <Eigen/Eigen>
#include <chrono>
#include <array>

namespace pcf {

/**
Viewer for a scene.
Handles smooth movement of camera through scene. Set up of OpenGL context and render loop needs to be handled externally. Contains scene.
*/
class viewer : public motion_controller {
private:
	using clock = std::chrono::high_resolution_clock;

	scene scene_;
	
	viewer(const viewer&) = delete;
	viewer& operator=(const viewer&) = delete;
	
public:
	viewer(std::size_t w, std::size_t h);
	~viewer();
	
	void resize_viewport(std::size_t w, std::size_t h);
	std::array<std::size_t, 2> viewport_size() const;

	void draw();
	
	void select_camera();
	void select_object(space_object&);
	
	scene* operator->() { return &scene_; }
	scene& operator*() { return scene_; }
};

}

#endif
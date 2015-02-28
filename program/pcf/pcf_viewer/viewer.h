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
Contains the scene with its objects. Handles smooth motion of selected object. This is either the camera (by default), or any space object. Through the space object observer mechanism, corresponding scene objects, or the camera, get updated with it.
*/
class viewer {
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

	/**
	Draw scene to current OpenGL context.
	Must be called when current OpenGL context is set up. Should be called repeatedly in short time intervals.
	*/
	void draw();
	
	/**
	Access to scene.
	*/
	scene* operator->() { return &scene_; }
	
	/**
	Access to scene.
	*/
	scene& operator*() { return scene_; }
};

}

#endif
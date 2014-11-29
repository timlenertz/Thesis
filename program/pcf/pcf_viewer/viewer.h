#ifndef PCFVW_VIEWER_H_
#define PCFVW_VIEWER_H_

#include "gl.h"
#include "scene/scene.h"
#include <Eigen/Eigen>
#include <chrono>
#include <array>

namespace pcf {

/**
Viewer for a scene.
Handles smooth movement of camera through scene. Set up of OpenGL context and render loop
needs to be handled externally.
*/
class viewer {
private:
	using clock = std::chrono::high_resolution_clock;

	scene scene_;
	Eigen::Vector3f velocity_ = Eigen::Vector3f::Zero();
	Eigen::Vector3f view_target_velocity_ = Eigen::Vector3f::Zero();
	std::chrono::time_point<clock> last_time_;
	
	void compute_motion_(std::chrono::milliseconds delta_t);
		
public:
	viewer(std::size_t w, std::size_t h);
	viewer(const viewer&) = delete;
	~viewer();
	
	viewer& operator=(const viewer&) = delete;

	void resize_viewport(std::size_t w, std::size_t h);
	std::array<std::size_t, 2> viewport_size() const;

	void draw();
	void tick();
	
	void rotate_camera(angle horizontal, angle vertical);
	void set_target_velocity(const Eigen::Vector3f& vel);
	void stop_movement();
	
	scene* operator->() { return &scene_; }
	scene& operator*() { return scene_; }
};

}

#endif
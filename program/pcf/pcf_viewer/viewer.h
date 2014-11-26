#ifndef PCFVW_VIEWER_H_
#define PCFVW_VIEWER_H_

#include "gl.h"
#include "scene/scene.h"
#include <Eigen/Eigen.h>
#include <chrono>

namespace pcf {

class scene;

/**
Viewer for a scene.
Handles smooth movement of camera through scene. Set up of OpenGL context and render loop
needs to be handled externally.
*/
class viewer {
private:
	using clock = std::chrono::high_resolution_clock;

	scene& scene_;
	Eigen::Vector3f velocity_ = Eigen::Vector3f::Zero();
	Eigen::Vector3f view_target_velocity_ = Eigen::Vector3f::Zero();
	std::chrono::time_point<clock> last_time_;
	
	void compute_motion_(std::chrono::milliseconds delta_t);
		
public:
	explicit viewer(scene& sc);
	viewer(const viewer&) = delete;
	~viewer();
	
	viewer& operator=(const viewer&) = delete;

	void draw();
	void tick();
	
	void set_target_velocity(const Eigen::Vector3f&);
	void stop_movement();
};

}

#endif
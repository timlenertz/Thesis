#ifndef PCFVW_VIEWER_H_
#define PCFVW_VIEWER_H_

#include "gl.h"
#include "scene/scene.h"
#include <Eigen/Eigen.h>

namespace pcf {

class viewer {
private:
	scene scene_;
	Eigen::Vector3f velocity_ = Eigen::Vector3f::Zero();
	Eigen::Vector3f view_target_velocity_ = Eigen::Vector3f::Zero();
		
	void compute_motion_();
		
public:
	void draw(float time_difference);
	void tick(float time_difference);
	
	void set_target_velocity(const Eigen::Vector3f&);
	
	scene& get_scene() { return scene_; }
};

}

#endif
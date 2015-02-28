#ifndef PCFVW_MOTION_CONTROLLER_H_
#define PCFVW_MOTION_CONTROLLER_H_

#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <chrono>
#include "../pcf_core/geometry/pose.h"
#include "../pcf_core/geometry/angle.h"

namespace pcf {

class space_object;
class camera;

/**
Controls smooth motion of space object over time.
Space object can be camera, or another object. Tick function needs to be called repeatedly. Converges towards set target velocity vector.
*/
class motion_controller {
private:
	using clock = std::chrono::high_resolution_clock;
	
	std::chrono::time_point<clock> last_time_;
	Eigen::Vector3f velocity_ = Eigen::Vector3f::Zero();
	const camera& camera_;

public:
	space_object* object = nullptr;
	Eigen::Vector3f target_velocity = Eigen::Vector3f::Zero();
	bool target_velocity_relative_to_camera = true;
	
	explicit motion_controller(const camera& cam) :
		camera_(cam) { }
	
	void tick();
	
	void rotate(angle horizontal, angle vertical);
	void roll(angle);
	void set_target_velocity(const Eigen::Vector3f&);
	void stop();
};

}

#endif
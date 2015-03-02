#include "motion_controller.h"
#include "../pcf_core/space_object.h"
#include "../pcf_core/camera/camera.h"


namespace pcf {

namespace {
	std::chrono::milliseconds maximal_time_step_(100);
	std::chrono::milliseconds minimal_time_step_(5);
	std::chrono::milliseconds time_to_target_velocity_(500);
	float minimal_movement_(0.01);
}


void motion_controller::tick() {
	using namespace std::chrono;

	// Exit if no object
	if(! object) return;

	// See how much time passed since last call
	time_point<clock> now = clock::now();
	milliseconds delta_t = duration_cast<milliseconds>(now - last_time_);
	last_time_ = now;

	// Bound time step (otherwise jumps occur when timer stalls)
	if(delta_t > maximal_time_step_) delta_t = maximal_time_step_;
	if(delta_t < minimal_time_step_) return;
	
	Eigen::Vector3f target_vel;
	if(target_velocity_relative_to_camera) {
		Eigen::Vector4f target_velocity_h(
			target_velocity[0], target_velocity[1], target_velocity[2], 0
		);
		target_vel = (camera_.absolute_pose().transformation_to_world() * target_velocity_h).head(3);
	} else {
		target_vel = target_velocity;
	}
	
	// Make velocity converge to target velocity
	Eigen::Vector3f velocity_difference = target_vel - velocity_;	
	if(delta_t > time_to_target_velocity_) velocity_ = target_vel;
	else velocity_ += (velocity_difference * delta_t.count()) / time_to_target_velocity_.count();
		
	// Move object according to current velocity and time difference
	Eigen::Vector3f position_difference = velocity_ * delta_t.count();
	if(position_difference.norm() > minimal_movement_)
		object->move(position_difference); // Object pose observers get called
	else if(target_vel.isZero())
		velocity_.setZero();
}



void motion_controller::rotate(angle horizontal, angle vertical) {
	if(object) object->transform(
		Eigen::AngleAxisf(-vertical, Eigen::Vector3f::UnitX()) *
		Eigen::AngleAxisf(-horizontal, Eigen::Vector3f::UnitY())
	);
}


void motion_controller::roll(angle a) {
	if(object) object->rotate_z_axis(a);
}


void motion_controller::stop() {
	velocity_.setZero();
	target_velocity.setZero();
}


}
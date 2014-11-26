#include "viewer.h"
#include "gl.h"

namespace pcf {

namespace {
	std::chrono::milliseconds maximal_time_step_(100);
	float 
}


void viewer::compute_motion_(std::chrono::milliseconds delta_t) {
	// Upped-bound time step (otherwise jumps occur when timer stalls)
	if(delta_t > maximal_time_step_) delta_t = maximal_time_step_;
	
	// Transform target velocity vector to world space
	const camera& cam = scene_.get_camera();
	Eigen::Vector3f target_velocity = cam.view_transformation_inverse() * view_target_velocity_;

	// 
	Eigen::Vector3f velocity_difference = target_velocity - velocity_;
	
	velocity_ += (velocity_difference / );
	
	// Move camera according to current velocity and time difference
	const pose& ps = scene_.get_camera_pose();
	ps.position += velocity_ * delta_t.count();
	scene_.set_camera_pose(ps); // Information propagates to scene objects
}


viewer::viewer(scene& sc) :
	scene_(sc) { }
	


void viewer::draw(float time_difference) const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	scene_.draw();
}


void viewer::set_target_velocity(const Eigen::Vector3f& vw_vel) {
	view_target_velocity_ = vw_vel;
}


void viewer::stop_movement() {
	velocity_.setZero();
	view_target_velocity_.setZero();
}


}


	if(dtime > 0.01) dtime = 0.01;
			
	glm::vec3 target_velocity = glm::rotate(glm::inverse(orientation_), view_target_velocity_);
	target_velocity *= scale_;
	
	glm::vec3 velocity_difference = target_velocity - velocity_;
	velocity_difference /= 20.0;
	
	velocity_ += velocity_difference;
	
	position_ += velocity_ * dtime;

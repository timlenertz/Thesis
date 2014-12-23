#include "viewer.h"
#include "gl.h"
#include <Eigen/Geometry>
#include <iostream>

namespace pcf {

namespace {
	angle default_fov_ = angle::degrees(60);

	std::chrono::milliseconds maximal_time_step_(100);
	std::chrono::milliseconds minimal_time_step_(5);
	std::chrono::milliseconds time_to_target_velocity_(500);
	float minimal_movement_(0.01);
}


void viewer::compute_motion_(std::chrono::milliseconds delta_t) {
	// Upped-bound time step (otherwise jumps occur when timer stalls)
	if(delta_t > maximal_time_step_) delta_t = maximal_time_step_;
	if(delta_t < minimal_time_step_) return;
	
	// Transform target velocity vector to world space
	const camera& cam = scene_.get_camera();
	
	Eigen::Vector4f view_target_velocity_h;
	view_target_velocity_h.head(3) = view_target_velocity_;
	view_target_velocity_h[3] = 0;
	
	Eigen::Vector3f target_velocity = (cam.view_transformation().inverse() * view_target_velocity_h).head(3);
	
	// Make velocity converge to target velocity
	Eigen::Vector3f velocity_difference = target_velocity - velocity_;	
	if(delta_t > time_to_target_velocity_) velocity_ = target_velocity;
	else velocity_ += (velocity_difference * delta_t.count()) / time_to_target_velocity_.count();
		
	// Move camera according to current velocity and time difference
	Eigen::Vector3f position_difference = velocity_ * delta_t.count();
	if(position_difference.norm() > minimal_movement_) {
		pose ps = scene_.get_camera_pose();
		ps.position += velocity_ * delta_t.count();
		scene_.set_camera_pose(ps); // Information propagates to scene objects
	} else if(view_target_velocity_.isZero()) {
		velocity_.setZero();
	}
}


viewer::viewer(std::size_t w, std::size_t h) :
	scene_(w, h, default_fov_) { }


viewer::~viewer() { }

std::array<std::size_t, 2> viewer::viewport_size() const {
	return { scene_.get_camera().image_width(), scene_.get_camera().image_height() };
}


void viewer::resize_viewport(std::size_t w, std::size_t h) {
	scene_.set_camera_image_size(w, h);
}


void viewer::tick() {
	using namespace std::chrono;
	
	time_point<clock> now = clock::now();
	milliseconds delta_t = duration_cast<milliseconds>(now - last_time_);
	last_time_ = now;
	
	compute_motion_(delta_t);
}


void viewer::draw() {
	tick();	
	scene_.draw();
}


void viewer::rotate_camera(angle horizontal, angle vertical) {
	auto transformation =
		Eigen::AngleAxisf(-vertical, Eigen::Vector3f::UnitX()) *
		Eigen::AngleAxisf(-horizontal, Eigen::Vector3f::UnitY());
	
	pose ps = scene_.get_camera_pose();
	ps.orientation = ps.orientation * transformation;
	scene_.set_camera_pose(ps);
}


void viewer::roll_camera(angle a) {
	auto transformation = Eigen::AngleAxisf(a, Eigen::Vector3f::UnitZ());
		
	pose ps = scene_.get_camera_pose();
	ps.orientation = ps.orientation * transformation;
	scene_.set_camera_pose(ps);
}


void viewer::set_target_velocity(const Eigen::Vector3f& vw_vel) {
	view_target_velocity_ = vw_vel;
}


void viewer::stop_movement() {
	velocity_.setZero();
	view_target_velocity_.setZero();
}


}
#include "viewer.h"
#include "gl.h"
#include <Eigen/Geometry>
#include <iostream>

namespace pcf {

namespace {
	angle default_fov_ = angle::degrees(60.0f);

	std::chrono::milliseconds maximal_time_step_(100);
	std::chrono::milliseconds minimal_time_step_(5);
	std::chrono::milliseconds time_to_target_velocity_(2000);
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
	
	Eigen::Vector3f target_velocity = (cam.view_transformation_inverse() * view_target_velocity_h).head(3);
	
	// Make velocity converge to target velocity
	Eigen::Vector3f velocity_difference = target_velocity - velocity_;	
	if(true || delta_t > time_to_target_velocity_) velocity_ = target_velocity;
	else velocity_ += (velocity_difference * delta_t.count()) / time_to_target_velocity_.count();
		
	// Move camera according to current velocity and time difference
	pose ps = scene_.get_camera_pose();
	ps.position += velocity_ * delta_t.count();
	scene_.set_camera_pose(ps); // Information propagates to scene objects
}


viewer::viewer(std::size_t w, std::size_t h) :
	scene_(w, h, default_fov_) { }


viewer::~viewer() { }

std::array<std::size_t, 2> viewer::viewport_size() const {
	return scene_.get_camera().get_image_size();
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	scene_.draw();
}


void viewer::rotate_camera(angle horizontal, angle vertical) {
	auto transformation =
		Eigen::AngleAxisf(horizontal, Eigen::Vector3f::UnitY()) *
		Eigen::AngleAxisf(vertical, Eigen::Vector3f::UnitX());
	
	pose ps = scene_.get_camera_pose();
	ps.orientation = transformation * ps.orientation;
	scene_.set_camera_pose(ps);
}

void viewer::set_target_velocity(const Eigen::Vector3f& vw_vel) {
	view_target_velocity_ = vw_vel;
	std::cout << view_target_velocity_ << std::endl;
}


void viewer::stop_movement() {
	velocity_.setZero();
	view_target_velocity_.setZero();
}


}
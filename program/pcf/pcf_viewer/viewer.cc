#include "viewer.h"
#include "gl.h"
#include <Eigen/Geometry>
#include <iostream>

namespace pcf {

namespace {
	angle default_fov_ = angle::degrees(60);
}

viewer::viewer(std::size_t w, std::size_t h) :
scene_(w, h, default_fov_) {
	select_camera();
}

viewer::~viewer() { }

std::array<std::size_t, 2> viewer::viewport_size() const {
	return { scene_.get_camera().image_width(), scene_.get_camera().image_height() };
}


void viewer::resize_viewport(std::size_t w, std::size_t h) {
	scene_.get_camera().set_image_size(w, h);
}


void viewer::draw() {
	motion_controller::tick();	
	scene_.draw();
}


void viewer::select_camera() {
	motion_controller::object = &scene_.get_camera();
}


void viewer::select_object(space_object& obj) {
	motion_controller::object = &obj;
}



}
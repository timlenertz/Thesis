#include "viewer.h"
#include "gl.h"
#include <Eigen/Geometry>
#include <iostream>

namespace pcf {

namespace {
	angle default_fov_ = angle::degrees(60);
}

viewer::viewer(std::size_t w, std::size_t h) :
scene_(w, h, default_fov_) { }

viewer::~viewer() { }

std::array<std::size_t, 2> viewer::viewport_size() const {
	return { scene_.camera().image_width(), scene_.camera().image_height() };
}


void viewer::resize_viewport(std::size_t w, std::size_t h) {
	scene_.camera().set_image_size(w, h);
	scene_.camera().adjust_field_of_view_y();
}


void viewer::draw() {
	scene_.draw();
}



}
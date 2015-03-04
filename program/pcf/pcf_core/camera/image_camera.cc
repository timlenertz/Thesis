#include "image_camera.h"
#include <iostream>
namespace pcf {

image_camera::image_camera(std::size_t imw, std::size_t imh) :
	image_width_(imw), image_height_(imh) { }


float image_camera::image_aspect_ratio() const {
	return float(image_width_) / image_height_;
}

void image_camera::set_image_size(std::size_t imw, std::size_t imh) {
	image_width_ = imw;
	image_height_ = imh;
}

void image_camera::set_image_width(std::size_t imw) {
	image_height_ = (float)imw / image_aspect_ratio();
	image_width_ = imw;
}


void image_camera::set_image_height(std::size_t imh) {
	image_width_ = image_aspect_ratio() * imh;
	image_height_ = imh;
}


std::size_t image_camera::image_number_of_pixels() const {
	return image_width_ * image_height_;
}


bool image_camera::in_bounds(image_coordinates ic) const {
	return (ic[0] >= 0) && (ic[0] < image_width_) && (ic[1] >= 0) && (ic[1] < image_height_);
}


}
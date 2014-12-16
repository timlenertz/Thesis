#include "image_camera.h"

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

}
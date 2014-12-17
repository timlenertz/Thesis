#include "image.h"

namespace pcf {

image::image(std::size_t w, std::size_t h, int type) :
	matrix_(h, w, type) { }

	
std::size_t image::width() const {
	return matrix_.cols;
}


std::size_t image::height() const {
	return matrix_.rows;
}


}
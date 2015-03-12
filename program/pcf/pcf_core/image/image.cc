#include "image.h"

namespace pcf {

image::image(std::size_t w, std::size_t h, int type) :
	matrix_(h, w, type) { }


image::image(const image& img) :
	matrix_(img.matrix_.clone()) { }
	
image::image(image&& img) :
	matrix_(img.matrix_) { }

image& image::operator=(const image& img) {
	matrix_ = img.matrix_.clone();
	return *this;
}	
	
image& image::operator=(image&& img) {
	matrix_ = img.matrix_;
	return *this;
}

	
std::size_t image::width() const {
	return matrix_.cols;
}


std::size_t image::height() const {
	return matrix_.rows;
}


void image::flip(bool vertical, bool horizontal) {
	int code;
	if(vertical && horizontal) code = -1;
	else if(vertical) code = 0;
	else if(horizontal) code = 1;
	else return;
	
	cv::flip(matrix_, matrix_, code);
}


}
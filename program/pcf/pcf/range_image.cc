#include "range_image.h"
#include <opencv2/opencv.hpp>
#include <cstdint>

namespace pcf {

range_image::range_image(std::size_t w, std::size_t h) :
matrix_(h, w, CV_32FC1, 0.0f) { }

void range_image::erase() {
	matrix_ = 0.0f;
}

std::size_t range_image::number_of_pixels() const {
	return matrix_.rows * matrix_.cols;
}

std::size_t range_image::width() const {
	return matrix_.cols;
}

std::size_t range_image::height() const {
	return matrix_.rows;
}

std::pair<float, float> range_image::minimum_and_maximum() const {
	float mn = INFINITY;
	float mx = 0;
	for(cv::MatConstIterator_<float> it = matrix_.begin<float>(); it != matrix_.end<float>(); ++it) {
		float d = *it;
		if(d == 0) continue;
		
		if(d < mn) mn = d;
		else if(d > mx) mx = d;
	}
	return std::make_pair(mn, mx);
}


void range_image::save_image(const std::string& path) {
	auto min_max = minimum_and_maximum();
	float mn = min_max.first, mx = min_max.second;
	float rn = mx - mn;
		
	cv::Mat img(matrix_.size(), CV_16UC1);
	
	cv::MatIterator_<std::uint16_t> ot = img.begin<std::uint16_t>();
	for(cv::MatConstIterator_<float> it = matrix_.begin<float>(); it != matrix_.end<float>(); ++it, ++ot) {
		float d = *it;
		if(d != 0) *ot = 0xffff - (d - mn)*0xffff / rn;
	}
	
	cv::imwrite(path, img);
}

}
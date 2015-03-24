#include "intensity_image.h"
#include <opencv2/opencv.hpp>
#include <cstdint>
#include <cmath>

namespace pcf {

intensity_image::intensity_image(std::size_t w, std::size_t h) :
	image(w, h, CV_32FC1) { }


std::pair<float, float> intensity_image::minimum_and_maximum() const {
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


void intensity_image::export_visualization_to_image_file(const std::string& path) const {
	auto min_max = minimum_and_maximum();
	float mn = min_max.first, mx = min_max.second;
	float rn = mx - mn;
		
	cv::Mat img(matrix_.size(), CV_16UC1, 0.0f);
	
	cv::MatIterator_<std::uint16_t> ot = img.begin<std::uint16_t>();
	for(cv::MatConstIterator_<float> it = matrix_.begin<float>(); it != matrix_.end<float>(); ++it, ++ot) {
		float d = *it;
		if(d != 0) *ot = 0xffff - (d - mn)*0xffff / rn;
	}
	
	cv::imwrite(path, img);
}


}
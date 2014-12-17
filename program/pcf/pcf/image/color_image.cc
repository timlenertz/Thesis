#include "color_image.h"
#include <opencv2/opencv.hpp>
#include <cmath>

namespace pcf {

color_image::color_image(std::size_t w, std::size_t h) :
	image(w, h, CV_8UC3) { }



void color_image::export_to_image_file(const std::string& path) const {
	// Need to convert channel order from RGB to BGR
	cv::Mat img(matrix_.size(), CV_8UC3);
	cv::cvtColor(matrix_, img, CV_RGB2BGR);	
	
	cv::imwrite(path, img);
}


}
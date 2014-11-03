#include "range_image.h"
#include <opencv2/opencv.hpp>

namespace pcf {

void range_image::save_image(const std::string& path) {
	cv::Mat img;
	cv::normalize(matrix_, img, 0, 0xffff, cv::NORM_MINMAX, CV_16UC1);
	cv::imwrite(path, img);
}

}
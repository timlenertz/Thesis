#include "range_image.h"

#include <opencv2/opencv.hpp>

namespace pcf {

void range_image::save(const char* str) {
	cv::imwrite(str, matrix_);
}

}
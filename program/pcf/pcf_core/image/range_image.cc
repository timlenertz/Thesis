#include "range_image.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <cstring>

namespace pcf {

bool range_image::contains_holes() const {
	using namespace cv;
	Mat mask = (matrix_ != invalid_value);
	float kernel_data[3][3] = { {-1, -1, -1}, {-1, +1, -1}, {-1, -1, -1} };
	Mat kernel(3, 3, CV_32FC1, kernel_data);
	filter2D(mask, mask, -1, kernel);
	Mat positive = (mask > 0);
	return (countNonZero(positive) > 0);
}

}
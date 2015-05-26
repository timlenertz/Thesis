#include "range_image.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <string>

namespace pcf {

bool range_image::contains_small_holes() const {
	using namespace cv;

	float kernel_data[3][3] = { {-1, -1, -1}, {-1, +1, -1} ,{-1, -1, -1} };
	Mat kernel(3, 3, CV_32FC1, kernel_data);
	Mat convolved;
	filter2D(super::mask_, convolved , -1, kernel);

	Mat positive = (convolved > 0);
	
	return (countNonZero(positive) > 0);
}

}
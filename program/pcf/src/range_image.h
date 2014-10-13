#ifndef PCF_RANGE_IMAGE_H_
#define PCF_RANGE_IMAGE_H_

#include <opencv2/core/core.hpp>
#include <cmath>
#include <string>
#include "asset.h"

namespace pcf {

class range_image : public asset {
private:
	cv::Mat matrix_;

public:
	range_image(std::size_t w, std::size_t h) :
	matrix_(h, w, CV_32FC1, NAN) { erase(); }
	
	void erase() { matrix_ = NAN; }
	
	float& at(std::ptrdiff_t x, std::ptrdiff_t y) { return matrix_.at<float>(y, x); }
	float at(std::ptrdiff_t x, std::ptrdiff_t y) const { return matrix_.at<float>(y, x); }
	
	void save_image(const std::string& path);
};

}

#endif
#ifndef PCF_RANGE_IMAGE_H_
#define PCF_RANGE_IMAGE_H_

#include <opencv2/core/core.hpp>
#include <cmath>
#include <string>
#include <utility>

namespace pcf {

class range_image {
private:
	cv::Mat matrix_;

public:
	range_image(std::size_t w, std::size_t h);
	
	void erase();
	
	std::size_t width() const;
	std::size_t height() const;
	std::size_t number_of_pixels() const;
	
	bool valid(std::ptrdiff_t x, std::ptrdiff_t y) const { return at(x, y) != 0.0f; }
	float& at(std::ptrdiff_t x, std::ptrdiff_t y) { return matrix_.at<float>(y, x); }
	const float& at(std::ptrdiff_t x, std::ptrdiff_t y) const { return matrix_.at<float>(y, x); }
	
	std::pair<float, float> minimum_and_maximum() const;
	
	void save_image(const std::string& path);
};

}

#endif
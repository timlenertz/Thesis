#ifndef PCF_INTENSITY_IMAGE_H_
#define PCF_INTENSITY_IMAGE_H_

#include "image.h"
#include <utility>
#include <string>

namespace pcf {

class intensity_image : public image {
public:
	static const float invalid_value;
	
	intensity_image(std::size_t w, std::size_t h);
	intensity_image(const intensity_image&) = default;
	intensity_image(const cv::Mat& mat) : image(mat) { }
	intensity_image& operator=(const intensity_image&) = default;

	bool valid(std::ptrdiff_t x, std::ptrdiff_t y) const;
	void invalidate(std::ptrdiff_t x, std::ptrdiff_t y);
	float& at(std::ptrdiff_t x, std::ptrdiff_t y);
	const float& at(std::ptrdiff_t x, std::ptrdiff_t y) const;
	
	std::pair<float, float> minimum_and_maximum() const;
	
	void export_visualization_to_image_file(const std::string& path) const;
};


	
inline bool intensity_image::valid(std::ptrdiff_t x, std::ptrdiff_t y) const {
	return at(x, y) != invalid_value;
}

inline void intensity_image::invalidate(std::ptrdiff_t x, std::ptrdiff_t y) {
	at(x, y) = invalid_value;
}

inline float& intensity_image::at(std::ptrdiff_t x, std::ptrdiff_t y) {
	return matrix_.at<float>(y, x);
}

inline const float& intensity_image::at(std::ptrdiff_t x, std::ptrdiff_t y) const {
	return matrix_.at<float>(y, x);
}


}

#endif
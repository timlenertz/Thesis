#ifndef PCF_RANGE_IMAGE_H_
#define PCF_RANGE_IMAGE_H_

#include "image.h"
#include <utility>
#include <string>

namespace pcf {

class range_image : public image {
public:
	range_image(std::size_t w, std::size_t h);
	range_image(const range_image&) = default;
	range_image& operator=(const range_image&) = default;

	bool valid(std::ptrdiff_t x, std::ptrdiff_t y) const;
	void invalidate(std::ptrdiff_t x, std::ptrdiff_t y);
	float& at(std::ptrdiff_t x, std::ptrdiff_t y);
	const float& at(std::ptrdiff_t x, std::ptrdiff_t y) const;
	
	std::pair<float, float> minimum_and_maximum() const;
	
	void export_visualization_to_image_file(const std::string& path) const;
};


	
inline bool range_image::valid(std::ptrdiff_t x, std::ptrdiff_t y) const {
	return at(x, y) != 0.0f;
}

inline void range_image::invalidate(std::ptrdiff_t x, std::ptrdiff_t y) {
	at(x, y) = 0.0f;
}

inline float& range_image::at(std::ptrdiff_t x, std::ptrdiff_t y) {
	return matrix_.at<float>(y, x);
}

inline const float& range_image::at(std::ptrdiff_t x, std::ptrdiff_t y) const {
	return matrix_.at<float>(y, x);
}


}

#endif
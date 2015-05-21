#ifndef PCF_RANGE_IMAGE_H_
#define PCF_RANGE_IMAGE_H_

#include "intensity_image.h"
#include <utility>
#include <string>

namespace pcf {

class range_image : public intensity_image {
public:
	range_image(std::size_t w, std::size_t h) : intensity_image(w, h) { }
	range_image(const range_image&) = default;
	range_image(const intensity_image& im) : intensity_image(im) { }
	range_image(const cv::Mat& mat) : intensity_image(mat) { }
	range_image& operator=(const range_image&) = default;
	
	bool contains_holes() const;
};

}

#endif

#ifndef PCF_COLOR_IMAGE_H_
#define PCF_COLOR_IMAGE_H_

#include "image.h"
#include "../rgb_color.h"
#include "rgb_color_opencv.h"
#include <utility>
#include <string>

namespace pcf {

class color_image : public image {
public:
	color_image(std::size_t w, std::size_t h);
	
	rgb_color& at(std::ptrdiff_t x, std::ptrdiff_t y);
	const rgb_color& at(std::ptrdiff_t x, std::ptrdiff_t y) const;
	
	void export_to_image_file(const std::string& path) const;
};


inline rgb_color& color_image::at(std::ptrdiff_t x, std::ptrdiff_t y) {
	return matrix_.at<rgb_color>(y, x);
}

inline const rgb_color& color_image::at(std::ptrdiff_t x, std::ptrdiff_t y) const {
	return matrix_.at<rgb_color>(y, x);
}


}

#endif
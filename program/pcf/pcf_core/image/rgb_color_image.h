#ifndef PCF_COLOR_IMAGE_H_
#define PCF_COLOR_IMAGE_H_

#include "image.h"
#include "../rgb_color.h"
#include <functional>

namespace pcf {

class rgb_color_image : public image<rgb_color, CV_8UC3> {
public:
	using image::image;
	
	static rgb_color_image import_from_memory(const void*, std::size_t);
	void export_to_image_file(const std::string& path) const;
	void export_png_to_memory(const std::function<void(const void*, std::size_t)>&) const;
	
	cv::Mat brg_opencv_matrix() const;
};

}

#endif
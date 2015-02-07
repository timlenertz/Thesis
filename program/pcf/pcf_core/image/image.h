#ifndef PCF_IMAGE_H_
#define PCF_IMAGE_H_

#include <opencv2/core/core.hpp>

namespace pcf {

class image {
protected:
	cv::Mat matrix_;

	image(std::size_t w, std::size_t h, int type);
	
public:
	std::size_t width() const;
	std::size_t height() const;
	
	void flip(bool vertical, bool horizontal);
};

}

#endif

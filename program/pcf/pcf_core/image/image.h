#ifndef PCF_IMAGE_H_
#define PCF_IMAGE_H_

#include <opencv2/core/core.hpp>

namespace pcf {

class image {
protected:
	cv::Mat matrix_;

	image(std::size_t w, std::size_t h, int type);
	image(const image&);
	image(image&&);
	image(const cv::Mat& mat) : matrix_(mat) { }
	image() = default;	

	image& operator=(const image&);
	image& operator=(image&&);

	
public:
	std::size_t width() const;
	std::size_t height() const;
	
	void flip(bool vertical, bool horizontal);
	
	const cv::Mat& opencv_matrix() const { return matrix_; }
	operator cv::Mat& () { return matrix_; }
	operator const cv::Mat& () const { return matrix_; }
};

}

#endif

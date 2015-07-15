#ifndef PCF_IMAGE_H_
#define PCF_IMAGE_H_

#include <opencv2/opencv.hpp>
#include "../util/coordinates.h"
#include "../util/multi_dimensional_buffer.h"

namespace pcf {

template<typename T, int Type>
class image {
protected:
	cv::Mat matrix_;
	
public:
	image(std::size_t w, std::size_t h);
	image() = default;	
	image(const image&);
	image(image&&);
	image(const cv::Mat&);
	
	explicit image(const multi_dimensional_buffer<T, 2>&);

	image& operator=(const image&);
	image& operator=(image&&);

	std::size_t width() const;
	std::size_t height() const;
	
	T& operator[](const index_2dim& ind) { return matrix_.at<T>(ind[1], ind[0]); }
	const T& operator[](const index_2dim& ind) const { return matrix_.at<T>(ind[1], ind[0]); }
	
	T at(const index_2dim& ind, T invalid = T()) const {
		if(in_bounds(ind)) return operator[](ind);
		else return invalid;
	}
	
	bool in_bounds(const index_2dim& ind) const { return in_bounds(ind.x, ind.y); }
	bool in_bounds(std::ptrdiff_t x, std::ptrdiff_t y) const;
	
	void flip(bool vertical, bool horizontal);
	
	const cv::Mat& operator*() const { return matrix_; }
	cv::Mat& operator*() { return matrix_; }
	operator cv::Mat& () { return matrix_; }
	operator const cv::Mat& () const { return matrix_; }
		
	void apply_transformation(const Eigen::Affine2f&);
	
	std::pair<T, T> minimum_and_maximum() const;
	void export_visualization_to_image_file(const std::string&) const;
	void normalize(T mn, T mx);
};

using intensity_image = image<float, CV_32FC1>;

}

#include "image.tcc"

#endif

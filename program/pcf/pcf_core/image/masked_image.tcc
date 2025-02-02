#include <iostream>

namespace pcf {


template<typename T, int Type>
masked_image<T, Type>::masked_image(std::size_t w, std::size_t h) :
	super(w, h), mask_(h, w, CV_8UC1, 1) { }


template<typename T, int Type>
masked_image<T, Type>::masked_image(const masked_image& img) :
	super(img), mask_(img.mask_.clone()) { }
	
	
template<typename T, int Type>
masked_image<T, Type>::masked_image(const super& img) :
	super(img), mask_(img.height(), img.width(), CV_8UC1, 1) { }
	
	
template<typename T, int Type>
masked_image<T, Type>::masked_image(const multi_dimensional_buffer<T, 2>& buf, const T& mask) :
masked_image(buf.size().x, buf.size().y) {
	index_2dim ic;
	for(ic.y = 0; ic.y != super::height(); ++ic.y)
	for(ic.x = 0; ic.x != super::width() ; ++ic.x) {
		T v = buf[ic];
		if(v == mask) {
			invalidate(ic);
		} else {
			super::operator[](ic) = buf[ic];
			validate(ic);
		}
	}
}

template<typename T, int Type>
masked_image<T, Type>& masked_image<T, Type>::operator=(const super& img) {
	super::operator=(img);
	return *this;
}	


template<typename T, int Type>
masked_image<T, Type>& masked_image<T, Type>::operator=(const masked_image& img) {
	super::operator=(img);
	mask_ = img.mask_.clone();
	return *this;
}	


template<typename T, int Type>
void masked_image<T, Type>::apply_transformation(const Eigen::Affine2f& t) {
	using namespace cv;
	cv::Mat m(2, 3, CV_32FC1);
	for(std::ptrdiff_t y = 0; y <= 1; ++y) for(std::ptrdiff_t x = 0; x <= 2; ++x)
		m.at<float>(x, y) = t.matrix()(x, y);
	cv::warpAffine(super::matrix_, super::matrix_, m, super::matrix_.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0.0);
	cv::warpAffine(mask_, mask_, m, mask_.size(), cv::INTER_NEAREST, cv::BORDER_CONSTANT, 0);
}


template<typename T, int Type>
std::pair<T, T> masked_image<T, Type>::minimum_and_maximum() const {
	double mn, mx;
	cv::minMaxLoc(super::matrix_, &mn, &mx, nullptr, nullptr, mask_);
	return std::make_pair(mn, mx);
}


template<typename T, int Type>
void masked_image<T, Type>::export_visualization_to_image_file(const std::string& path) const {
	cv::Mat img(super::matrix_.size(), CV_16UC1, 0.0f);
	cv::normalize(super::matrix_, img, 0x0000, 0xffff, cv::NORM_MINMAX, CV_16UC1, mask_);
	cv::imwrite(path, img);
}



template<typename T, int Type>
void masked_image<T, Type>::normalize(T mn, T mx) {
	cv::normalize(super::matrix_, super::matrix_, mn, mx, cv::NORM_MINMAX, -1, mask_);
}


}
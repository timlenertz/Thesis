namespace pcf {


template<typename T, int Type>
masked_image<T, Type>::masked_image(std::size_t w, std::size_t h) :
	image(w, h), mask_(h, w, CV_8UC1, 1) { }


template<typename T, int Type>
masked_image<T, Type>::masked_image(const masked_image& img) :
	image(img), mask_(img.mask_.clone()) { }
	
	
template<typename T, int Type>
masked_image<T, Type>::masked_image(const super& img) :
	image(img), mask_(img.height(), img.width(), CV_8UC1, 1) { }
	
	

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
	cv::warpAffine(matrix_, matrix_, m, matrix_.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0.0);
	cv::warpAffine(mask_, mask_, m, mask_.size(), cv::INTER_NEAREST, cv::BORDER_CONSTANT, 0)
}


template<typename T, int Type>
std::pair<T, T> masked_image<T, Type>::minimum_and_maximum() const {
	double mn, mx;
	cv::minMaxLoc(matrix_, &mn, &mx, nullptr, nullptr, mask_);
	return std::make_pair(mn, mx);
}


template<typename T, int Type>
void masked_image<T, Type>::export_visualization_to_image_file(const std::string& path) const {
	auto min_max = minimum_and_maximum();
	double mn = min_max.first, mx = min_max.second;

	cv::Mat img(matrix_.size(), CV_16UC1, 0.0f);
	cv::normalize(matrix_, img, mn, mx, cv::NORM_MINMAX, 1, mask_);
	cv::imwrite(path, img);
}



template<typename T, int Type>
void masked_image<T, Type>::normalize(T mn, T mx) {
	cv::normalize(matrix_, matrix_, mn, mx, cv::NORM_MINMAX, -1, mask_);
}


}
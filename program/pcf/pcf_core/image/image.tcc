namespace pcf {

template<typename T, int Type>
image<T, Type>::image(std::size_t w, std::size_t h) :
	matrix_(h, w, Type) { }


template<typename T, int Type>
image<T, Type>::image(const image& img) :
	matrix_(img.matrix_.clone()) { }
	
	
template<typename T, int Type>
image<T, Type>::image(image&& img) :
	matrix_(img.matrix_) { }


template<typename T, int Type>
image<T, Type>::image(const multi_dimensional_buffer<T, 2>& buf) :
image(buf.size().x, buf.size().y) {
	index_2dim ic;
	for(ic.y = 0; ic.y != height(); ++ic.y) for(ic.x = 0; ic.x != width(); ++ic.x)
		operator[](ic) = buf[ic];
}


template<typename T, int Type>
image<T, Type>& image<T, Type>::operator=(const image& img) {
	matrix_ = img.matrix_.clone();
	return *this;
}	
	
	
template<typename T, int Type>
image<T, Type>& image<T, Type>::operator=(image&& img) {
	matrix_ = img.matrix_;
	return *this;
}


template<typename T, int Type>
image<T, Type>::image(const cv::Mat& mat) :
	matrix_(mat.clone()) { }


template<typename T, int Type>
std::size_t image<T, Type>::width() const {
	return matrix_.cols;
}


template<typename T, int Type>
std::size_t image<T, Type>::height() const {
	return matrix_.rows;
}


template<typename T, int Type>
void image<T, Type>::flip(bool vertical, bool horizontal) {
	int code;
	if(vertical && horizontal) code = -1;
	else if(vertical) code = 0;
	else if(horizontal) code = 1;
	else return;
	
	cv::flip(matrix_, matrix_, code);
}


template<typename T, int Type>
bool image<T, Type>::in_bounds(std::ptrdiff_t x, std::ptrdiff_t y) const {
	return (x >= 0 && y >= 0 && x < matrix_.rows && y < matrix_.cols);
}


template<typename T, int Type>
void image<T, Type>::apply_transformation(const Eigen::Affine2f& t) {
	using namespace cv;
	cv::Mat m(2, 3, CV_32FC1);
	for(std::ptrdiff_t y = 0; y <= 1; ++y) for(std::ptrdiff_t x = 0; x <= 2; ++x)
		m.at<float>(x, y) = t.matrix()(x, y);
	cv::warpAffine(matrix_, matrix_, m, matrix_.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0.0);
}


template<typename T, int Type>
std::pair<T, T> image<T, Type>::minimum_and_maximum() const {
	double mn, mx;
	cv::minMaxLoc(matrix_, &mn, &mx, nullptr, nullptr);
	return std::make_pair(mn, mx);
}


template<typename T, int Type>
void image<T, Type>::export_visualization_to_image_file(const std::string& path) const {
	cv::Mat img(matrix_.size(), CV_16UC1, 0.0f);
	cv::normalize(matrix_, img, 0x0000, 0xffff, cv::NORM_MINMAX, 1);
	cv::imwrite(path, img);
}



template<typename T, int Type>
void image<T, Type>::normalize(T mn, T mx) {
	cv::normalize(matrix_, matrix_, mn, mx, cv::NORM_MINMAX, -1);
}


}
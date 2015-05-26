#include "rgb_color_image.h"
#include <cmath>
#include <cstring>

namespace pcf {


rgb_color_image rgb_color_image::import_from_memory(const void* raw_data, std::size_t sz) {
	void* raw_data_copy = std::malloc(sz);
	try {
		std::memcpy(raw_data_copy, raw_data, sz);

		cv::Mat data_mat(sz, 1, CV_8U, raw_data_copy);
		cv::Mat img_mat = cv::imdecode(data_mat, 1);
		std::free(raw_data_copy);

		color_image img;
		img.matrix_ = cv::Mat(img_mat.size(), CV_8UC3);
		cv::cvtColor(img_mat, img.matrix_, CV_BGR2RGB);	
	
		return img;
	} catch(...) {
		std::free(raw_data_copy);
		throw;
	}
}


void rgb_color_image::export_to_image_file(const std::string& path) const {
	cv::imwrite(path, brg_opencv_matrix());
}


void rgb_color_image::export_png_to_memory(const std::function<void(const void*, std::size_t)>& cb) const {
	std::vector<uchar> buf;
	cv::imencode(".png", brg_opencv_matrix(), buf);
	cb( (const void*)buf.data(), buf.size() );
}


cv::Mat rgb_color_image::brg_opencv_matrix() const {
	cv::Mat mat(matrix_.size(), CV_8UC3);
	cv::cvtColor(matrix_, mat, CV_BGR2RGB);
	return mat;	
}


}
#include "run_result.h"
#include "../pcf_core/image/color_image.h"
#include <opencv2/opencv.hpp>
#include "../pcf_core/util/misc.h"

namespace pcf {
namespace exper {

run_result::state::state(state&&) = default;
run_result::state& run_result::state::state::operator=(state&&) = default;
run_result::state::~state() = default;


void run_result::export_animation(const std::string& filename, const char* format) const {
	double fps = 5;
	cv::VideoWriter vid;
	vid.open(
		filename,
		CV_FOURCC(format[0], format[1], format[2], format[3]),
		fps,
		evolution[0].snapshot->brg_opencv_matrix().size(),
		true
	);
	for(const state& st : evolution)
		vid << st.snapshot->brg_opencv_matrix();
}

}
}
#include "projected_range_image.h"
#include <opencv2/opencv.hpp>

namespace pcf {

void projected_range_image::translate_z(float z) {
	for(cv::MatIterator_<float> it = matrix_.begin<float>(); it != matrix_.end<float>(); ++it) {
		if(*it == 0) continue;
		*it = *it + z;
	}
}

}

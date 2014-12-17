#ifndef PCF_RGB_COLOR_OPENCV_H_
#define PCF_RGB_COLOR_OPENCV_H_

#include "../rgb_color.h"
#include <opencv2/core/core.hpp>
#include <cstdint>

namespace cv { // In OpenCV namespace...

/// Traits of rgb_color class for usage with OpenCV.
template<> class DataType<pcf::rgb_color> {
public:
	using value_type = pcf::rgb_color;
	using work_type = int;
	using channel_type = std::uint8_t;
	enum {
		generic_type = 0,
		depth = DataDepth<channel_type>::value,
		channels = 3,
		fmt = ((3 - 1)<<8) + DataDepth<channel_type>::fmt,
		type = CV_MAKETYPE(depth, channels)
	};
	using vec_type = Vec<channel_type, channels>;
};

}

#endif
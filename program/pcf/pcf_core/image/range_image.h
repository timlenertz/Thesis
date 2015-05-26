#ifndef PCF_RANGE_IMAGE_H_
#define PCF_RANGE_IMAGE_H_

#include "masked_image.h"
#include <utility>
#include <string>

namespace pcf {

class range_image : public masked_intensity_image {
	using super = masked_intensity_image;

public:
	using masked_intensity_image::masked_intensity_image;
		
	bool contains_small_holes() const;
};

}

#endif

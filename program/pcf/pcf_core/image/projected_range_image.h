#ifndef PCF_PROJECTED_RANGE_IMAGE_H_
#define PCF_PROJECTED_RANGE_IMAGE_H_

#include "range_image.h"
#include <utility>
#include <string>

namespace pcf {

class projected_range_image : public range_image {
public:
	using range_image::range_image;
	
	void translate_z(float z);
};

}

#endif

#ifndef PCF_POINT_CLOUD_ALLOC_H_
#define PCF_POINT_CLOUD_ALLOC_H_

#include <type_traits>
#include "point_cloud.h"

namespace pcf {

template<typename Point>
class point_cloud_alloc : public point_cloud<Point> {
public:
	explicit point_cloud_alloc(std::size_t size) : 
		point_cloud<Point>(new Point [size], size) { }
	
	~point_cloud_alloc() override {
		delete[] this->buffer_;
	}
};

}

#endif
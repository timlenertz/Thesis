#ifndef PCF_POINT_CLOUD_ARRAY_H_
#define PCF_POINT_CLOUD_ARRAY_H_

#include <memory>
#include "point_cloud.h"

namespace pcf {

template<typename Point, typename Allocator = std::allocator<Point>>
class point_cloud_array : public point_cloud<Point> {
private:
	Allocator allocator_;
	
public:
	explicit point_cloud_array(std::size_t n) {
		buffer_ = allocator_.allocate(n);
		buffer_end = buffer_ + n;
	}
	
	~point_cloud_array() {
		allocator_.deallocate(buffer_, size());
	}
};

}

#endif
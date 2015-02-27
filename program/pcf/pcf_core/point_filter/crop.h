#ifndef PCF_CROP_POINT_FILTER_H_
#define PCF_CROP_POINT_FILTER_H_

#include <utility>
#include "point_filter.h"

namespace pcf {

/**
Accept only points inside the given geometric object.
For example bounding box.
*/
template<typename Geometry_object>
class crop_point_filter : public point_filter {
private:
	Geometry_object object_;

public:
	template<typename... Args>
	crop_point_filter(Args&&... args) :
		object_(std::forward<Args>(args)...) { }
	
	bool operator()(const point_xyz& pt) {
		return object_.contains(pt.coordinates());
	}
};

}

#endif
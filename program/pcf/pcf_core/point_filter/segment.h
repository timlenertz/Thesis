#ifndef PCF_SEGMENT_POINT_FILTER_H_
#define PCF_SEGMENT_POINT_FILTER_H_

#include "point_filter.h"
#include "../point_cloud/segment.h"

namespace pcf {

/**
Accept only points in given point cloud segment.
*/
template<typename Point>
class segment_point_filter : public point_filter {
public:
	using segment_type = point_cloud_segment<Point>;

private:
	segment_type segment_;

public:
	segment_point_filter(const segment_type& seg) :
		segment_(seg) { }
		
	bool operator()(const Point& pt) const {
		return segment_.contains(pt);
	}
};

}

#endif

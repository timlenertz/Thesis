#ifndef PCF_OCTREE_POINT_CLOUD_H_
#define PCF_OCTREE_POINT_CLOUD_H_

#include "point_cloud.h"

namespace pcf {

template<typename Point, typename Allocator = std::allocator<Point>>
class octree_point_cloud : public point_cloud<Point, Allocator> {
private:
	using super = point_cloud<Point, Allocator>;
	
private:
	

};

}

#endif
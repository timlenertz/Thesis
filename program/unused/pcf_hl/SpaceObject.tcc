#include "../pcf/point_cloud/unorganized/unorganized_point_cloud.h"
#include "../pcf/point_cloud/range/range_point_cloud.h"
#include "../pcf/point_cloud/grid/grid_point_cloud.h"

#include <typeinfo>

#define ATTEMPT(Type) \
	if(Type* pc = dynamic_cast<Type*>(object_)) { \
		cb(*pc); \
		return; \
	}

template<class Callback>
void SpaceObject::accept(Callback& cb) {
	// Passing from static polymorphism to dynamic polymorphism
	std::type_info type = typeid(*object_);
	ATTEMPT(unorganized_point_cloud_full);
	ATTEMPT(range_point_cloud_full);
	ATTEMPT(grid_point_cloud_full);
	ATTEMPT(point_cloud_full);
	throw std::logic_error("Space object is not a (full) point cloud.");
}

#undef ATTEMPT


inline point_cloud_full& SpaceObject::point_cloud() {
	return dynamic_cast<point_cloud_full&>(*object_);
}


inline const point_cloud_full& SpaceObject::point_cloud() const {
	return dynamic_cast<const point_cloud_full&>(*object_);
}

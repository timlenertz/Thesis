#ifndef PCF_SPACE_OBJECT_WRAPPER_H_
#define PCF_SPACE_OBJECT_WRAPPER_H_

#include "space_object.h"
#include "geometry/bounding_box.h"
#include "geometry/plane.h"
#include "geometry/frustum.h"
#include <utility>

namespace pcf {

/**
Wraps any class into a space object.
Used for adding space object attributes (pose, etc) to primitive object such as bounding box, frustum, etc.
*/
template<typename T>
class space_object_wrapper : public space_object {
private:
	T object_;

public:
	template<typename... Args>
	space_object_wrapper(Args&&... args) :
		object_(std::forward<Args>(args)...) { }
	
	T& operator*() { return object_; }
	const T& operator*() const { return object_; }
	T* operator->() { return &object_; }
	const T* operator->() const { return &object_; }
	
	void apply_pose() {
		const Eigen::Affine3f& t = relative_pose().transformation_to_world();
		object_.apply_transformation(t);
		set_no_relative_pose();
		handle_update();
	}
};

using space_bounding_box = space_object_wrapper<bounding_box>;
using space_plane = space_object_wrapper<plane>;
using space_frustum = space_object_wrapper<frustum>;

}

#endif

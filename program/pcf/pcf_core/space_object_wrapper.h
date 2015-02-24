#ifndef PCF_SPACE_OBJECT_WRAPPER_H_
#define PCF_SPACE_OBJECT_WRAPPER_H_

#include "space_object.h"
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
};

}

#endif

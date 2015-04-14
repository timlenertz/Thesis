#include "TransformUnit.h"

namespace pcf {

SpaceObjectPtr TransformUnit::pull() {
	SpaceObjectPtr ptr = object_;
	object_.reset();
	return ptr;
}

void TransformUnit::push(SpaceObjectPtr obj) {
	object_ = obj;
}

}

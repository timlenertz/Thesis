#include "SourceUnit.h"

namespace pcf {

SpaceObjectPtr SourceUnit::pull() {
	SpaceObjectPtr ptr = object_;
	object_.reset();
	return ptr;
}

}

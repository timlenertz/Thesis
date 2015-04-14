#ifndef PCFHL_SOURCE_UNIT_H_
#define PCFHL_SOURCE_UNIT_H_

#include "Unit.h"
#include "SpaceObject.h"
#include <memory>

namespace pcf {

class TransformUnit : public Unit {
protected:
	SpaceObjectPtr object_;
	
public:
	void push(SpaceObjectPtr);
	SpaceObjectPtr pull();
};


}

#endif
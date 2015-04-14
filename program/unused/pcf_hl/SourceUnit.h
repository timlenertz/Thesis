#ifndef PCFHL_SOURCE_UNIT_H_
#define PCFHL_SOURCE_UNIT_H_

#include "Unit.h"
#include <memory>

namespace pcf {

class SpaceObject;

class SourceUnit : public Unit {
protected:
	SpaceObjectPtr object_;
	
public:
	SpaceObjectPtr pull();
};


}

#endif
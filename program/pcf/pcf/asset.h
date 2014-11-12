#ifndef PCF_ASSET_H_
#define PCF_ASSET_H_

#include "geometry/pose.h"

namespace pcf {

class asset {
private:
	asset* reference_ = nullptr;
	pose pose_;
	enum { none, coarse, fine, exact } aligmnent_quality_;
	
public:
	virtual ~asset() { }
};

}

#endif
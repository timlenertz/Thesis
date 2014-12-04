#ifndef PCF_ASSET_H_
#define PCF_ASSET_H_

#include "geometry/pose.h"

namespace pcf {

class asset {
protected:
	pose pose_;
	
	asset() = default;
	explicit asset(const pose& ps) : pose_(ps) { }

public:
	const pose& get_pose() const;
	void set_pose(const pose&);
	void clear_pose();
};

}

#endif

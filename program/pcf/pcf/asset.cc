#include "asset.h"

namespace pcf {

const pose& asset::get_pose() const {
	return pose_;
}

void asset::set_pose(const pose& ps) {
	pose_ = ps;
}

void asset::clear_pose() {
	pose_ = pose();
}


}
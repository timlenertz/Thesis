#include "pose.h"

namespace pcf {

pose::pose() {
	position.setZero();
	orientation.setIdentity();
}

Eigen::Affine3f pose::view_transformation() const {
	return orientation.conjugate() * Eigen::Translation3f(-position);
}


}
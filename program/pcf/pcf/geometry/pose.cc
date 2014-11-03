#include "pose.h"

namespace pcf {

pose::pose() {
	position.setZero();
	orientation.setIdentity();
}

Eigen::Affine3f pose::transformation() const {
	return Eigen::Translation3f(position) * orientation;
}

Eigen::Matrix4f pose::matrix() const {
	return transformation.matrix();
}

}
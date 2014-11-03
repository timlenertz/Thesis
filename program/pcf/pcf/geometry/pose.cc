#include "pose.h"

namespace pcf {

pose::pose() {
	position.setZero();
	orientation.setIdentity();
}

Eigen::Affine3f pose::view_transformation() const {
	return orientation.normalized().inverse() * Eigen::Translation3f(-position);
}

Eigen::Matrix4f pose::view_matrix() const {
	return view_transformation().matrix();
}

}
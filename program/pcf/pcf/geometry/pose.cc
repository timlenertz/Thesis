#include "pose.h"

namespace pcf {

pose::pose() {
	position.setZero();
	orientation.setIdentity();
}

Eigen::Affine3f pose::view_transformation() const {
	return Eigen::Translation3f(-position) * orientation.normalized().inverse();
}

Eigen::Matrix4f pose::view_matrix() const {
	return transformation.matrix();
}

}
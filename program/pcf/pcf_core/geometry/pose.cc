#include "pose.h"

namespace pcf {

pose::pose() :
	position(Eigen::Vector3f::Zero()),
	orientation(Eigen::Quaternionf::Identity()) { }



pose::pose(const Eigen::Affine3f& t) :
	position(t.translation()),
	orientation(t.rotation()) { }


Eigen::Affine3f pose::transformation_from_world() const {
	return orientation.conjugate() * Eigen::Translation3f(-position);
}

Eigen::Affine3f pose::transformation_to_world() const {
	return Eigen::Translation3f(position) * orientation;
}

pose pose::transform(const Eigen::Affine3f& t) const {
	return pose(
		position + t.translation(),
		orientation * t.rotation()
	);
}

}

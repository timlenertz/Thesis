#include "pose.h"
#include "angle.h"

namespace pcf {

pose::pose() :
	position(Eigen::Vector3f::Zero()),
	orientation(Eigen::Quaternionf::Identity()) { }



pose::pose(const Eigen::Affine3f& t) :
	position(t.translation()),
	orientation(t.rotation()) {
}


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

Eigen::Vector3f pose::euler_angles(std::ptrdiff_t a0, std::ptrdiff_t a1, std::ptrdiff_t a2) const {
	return orientation.toRotationMatrix().eulerAngles(a0, a1, a2);
}


std::ostream& operator<<(std::ostream& str, const pose& ps) {
	Eigen::Vector3f euler = ps.euler_angles();
	str << "position (" << ps.position[0] << ", " << ps.position[1] << ", " << ps.position[2] << "); "
	<< " orientation (" << angle(euler[0]) << ", " << angle(euler[1]) << ", " << angle(euler[2]) << ")";
	return str;
}

}

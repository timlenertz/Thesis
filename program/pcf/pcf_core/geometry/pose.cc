#include "pose.h"
#include "angle.h"
#include "../util/misc.h"
#include "../util/random.h"
#include "spherical_coordinates.h"
#include <stdexcept>

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

std::string pose::to_string() const {
	return implode_to_string<float>(',', {
		position[0],
		position[1],
		position[2],
		orientation.w(),
		orientation.x(),
		orientation.y(),
		orientation.z()
	});
}

pose pose::from_string(const std::string& str) {
	std::vector<float> p = explode_from_string<float>(',', str);
	if(p.size() != 7)
		throw std::invalid_argument("Invalid string to convert to pose.");
	
	Eigen::Vector3f position(p[0], p[1], p[2]);
	Eigen::Quaternionf orientation(p[3], p[4], p[5], p[6]);
	return pose(position, orientation);
}


pose pose::random_displacement(float translation_mag, angle rotation_mag) const {
	Eigen::Translation3f translation( spherical_coordinates::random_direction(translation_mag).to_cartesian() );
	
	Eigen::Vector3f rotation_axis = spherical_coordinates::random_direction().to_cartesian();
	Eigen::Affine3f rotation( Eigen::AngleAxisf(rotation_mag, rotation_axis) );
	
	return transform(translation * rotation);
}

}

#include "plane.h"
#include <vector>
#include "../util/misc.h"

namespace pcf {

plane::plane(float a, float b, float c, float d) :
normal(a, b, c), distance(d) {
	normalize();
}


plane::plane(const Eigen::Vector3f& p, const Eigen::Vector3f& n) :
normal(n), distance(-n.dot(p)) {
	normalize();
}


plane::plane(const Eigen::Vector3f& p1, const Eigen::Vector3f& p2, const Eigen::Vector3f& p3) :
plane(p1, (p2 - p1).cross(p3 - p1)) { }


plane::plane(const pose& ps) :
plane(ps.position, ps.orientation * Eigen::Vector3f::UnitY()) {}

void plane::normalize() {
	float norm = normal.norm();
	normal /= norm;
	distance /= norm;
}


Eigen::Vector3f plane::project(const Eigen::Vector3f& p) const {
	float a = normal[0], b = normal[1], c = normal[2];
	float k = a*p[0] + b*p[1] + c*p[2] - distance;
	return Eigen::Vector3f(p[0] - a*k, p[1] - b*k, p[2] - c*k);
}


Eigen::Vector3f plane::origin() const {
	return Eigen::Vector3f(normal[0]*distance, normal[1]*distance, normal[2]*distance);
}


float signed_distance(const Eigen::Vector3f& pt, const plane& pl) {
	return pl.normal.dot(pt) + pl.distance;
}


float distance(const Eigen::Vector3f& pt, const plane& pl) {
	float d = signed_distance(pt, pl);
	return (d > 0 ? d : -d);
}

pose plane::to_pose() const {
	Eigen::Vector3f translation = project(Eigen::Vector3f::Zero());
	Eigen::Quaternionf orientation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitY(), normal);
	return pose(translation, orientation);
}


void plane::apply_transformation(const Eigen::Affine3f& t) {
	normal = (t * Eigen::Vector4f(normal[0], normal[1], normal[2], 0.0)).head(3);
	distance += normal.dot(t.translation());
}

std::string plane::to_string() const {
	return implode_to_string<float>(',', {
		normal[0],
		normal[1],
		normal[2],
		distance
	});
}


plane plane::from_string(const std::string& str) {
	std::vector<float> p = explode_from_string<float>(',', str);
	if(p.size() != 4)
		throw std::invalid_argument("Invalid string to convert to plane.");
	
	return plane(p[0], p[1], p[2], p[3]);
}




}

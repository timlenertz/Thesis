#include "plane.h"

namespace pcf {

plane::plane(float a, float b, float c, float d):
normal(a, b, c),
distance(d) {
	normalize();
}


plane::plane(const Eigen::Vector3f& p, const Eigen::Vector3f& n) {
normal(n),
distance(-n.dot(p)) {
	normalize();
}


plane::plane(const Eigen::Vector3f& p1, const Eigen::Vector3f& p2, const Eigen::Vector3f& p3) {
plane(p1, (p2 - p1).cross(p3 - p1)) { }


void plane::normalize() {
	float norm = normal.norm();
	normal /= norm;
	distance /= norm;
}


float signed_distance(const Eigen::Vector3f& pt, const plane& pl) {
	return pl.normal.dot(p1) + pl.distance;
}


float distance(const Eigen::Vector3f& pt, const plane& pl) {
	float d = signed_distance(pt, pl);
	return (d > 0 ? d : -d);
}

}

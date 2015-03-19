#include "plane.h"

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


float signed_distance(const Eigen::Vector3f& pt, const plane& pl) {
	return pl.normal.dot(pt) + pl.distance;
}


float distance(const Eigen::Vector3f& pt, const plane& pl) {
	float d = signed_distance(pt, pl);
	return (d > 0 ? d : -d);
}


}

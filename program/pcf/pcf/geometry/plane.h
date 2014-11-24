#ifndef PCF_PLANE_H_
#define PCF_PLANE_H_

#include <Eigen/Eigen>

namespace pcf {

struct plane {
	Eigen::Vector3f normal = Eigen::Vector3f::Zero();
	float distance = 0;
	
	plane() = default;
	plane(const plane&) = default;
	
	plane(float a, float b, float c, float d);
	plane(const Eigen::Vector3f& p, const Eigen::Vector3f& n);
	plane(const Eigen::Vector3f& p1, const Eigen::Vector3f& p2, const Eigen::Vector3f& p3);
	
	void normalize();
};


float signed_distance(const Eigen::Vector3f&, const plane&);
float distance(const Eigen::Vector3f&, const plane&);


}

#endif
#ifndef PCF_PLANE_H_
#define PCF_PLANE_H_

#include <Eigen/Eigen>

namespace pcf {

/**
Oriented plane in 3D space.
Represented using normal vector and distance value. That is the values (a, b, c, d) in the plane equation ax + by + cz = d. When normalized, representations are unique and distance is from origin to plane.
*/
struct plane {
	Eigen::Vector3f normal = Eigen::Vector3f::Zero();
	float distance = 0;
	
	plane() = default;
	plane(const plane&) = default;
	
	/// Create from coefficients.
	/// Creates plane for equation ax + by + cz = d. Resulting plane is normalized.
	plane(float a, float b, float c, float d);
	
	/// Create from any point on plane and normal vector.
	/// Normal vector does not need to be normalized. Resulting plane is normalized.
	plane(const Eigen::Vector3f& p, const Eigen::Vector3f& n);
	
	/// Create any three points on the plane.
	/// Resulting plane is normalized. Points must not be aligned or coincide.
	plane(const Eigen::Vector3f& p1, const Eigen::Vector3f& p2, const Eigen::Vector3f& p3);
	
	/// Normalize the plane representation.
	void normalize();
	
	/// Project point on plane.
	/// Result is the point on plane that is closest to the given point.
	Eigen::Vector3f project(const Eigen::Vector3f&) const;
};


float signed_distance(const Eigen::Vector3f&, const plane&);
float distance(const Eigen::Vector3f&, const plane&);


}

#endif
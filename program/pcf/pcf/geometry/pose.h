#ifndef PCF_POSE_H_
#define PCF_POSE_H_

#include <Eigen/Geometry>
#include <iostream>

namespace pcf {

/**
Position and orientation in space.
Represented in terms of a position, and orientation in world space. The orientation is in the translated space, i.e. a pose of an object means that the object was first translated to pose.position, then rotated by pose.orientation. A pose also defines an orthonormal coordinate system, aka the world as seen from a camera at that pose. view_transformation gives a transformation matrix from world space to the pose's coordinate system.
*/
class pose {
public:
	Eigen::Vector3f position; ///< Position vector.
	Eigen::Quaternionf orientation; ///< Orientation quaternion. Must be kept normalized.

	/// Create identity pose.
	pose() :
		position(Eigen::Vector3f::Zero()),
		orientation(Eigen::Quaternionf::Identity()) { }
	
	/// Copy-construct pose.
	pose(const pose&) = default;
	
	template<typename Translation, typename Rotation>
	pose(const Translation& t, const Rotation& r) :
		position(t),
		orientation(r) { }
	
	
	/// Affine transformation from world space to pose coordinate system.
	Eigen::Affine3f view_transformation() const {
		return orientation.conjugate() * Eigen::Translation3f(-position);
	}

	/// Affine transformation from pose coordinate system to world space.
	Eigen::Affine3f view_transformation_inverse() const {
		return Eigen::Translation3f(position) * orientation;
	}
	
	/// Apply affine transformation to pose.
	void transform(const Eigen::Affine3f& t) {
		position += t.translation();
		orientation = orientation * t.rotation();
	}
};

}

#endif
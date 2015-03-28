#ifndef PCF_POSE_H_
#define PCF_POSE_H_

#include <Eigen/Geometry>
#include <ostream>
#include <string>
#include "angle.h"

namespace pcf {

/**
Position and orientation in space.
Represented in terms of a position, and orientation in world space. The orientation is in the translated space, i.e. a pose of an object means that the object was first translated to pose.position, then rotated by pose.orientation. A pose also defines an orthonormal coordinate system, aka the world as seen from a camera at that pose.
*/
class pose {
public:
	/// Position vector.
	Eigen::Vector3f position;
	
	/// Orientation quaternion. Must be kept normalized.
	Eigen::Quaternionf orientation;

	/// Create identity pose.
	pose();
		
	/// Copy-construct pose.
	pose(const pose&) = default;
	
	/// Construct pose from affine transformation.
	/// Transformation is transformation to world. Assumes that the transformation consists only of translation and rotation.
	pose(const Eigen::Affine3f&);
	
	// Construct pose with given translation and rotation.
	template<typename Translation, typename Rotation>
	pose(const Translation& t, const Rotation& r) :
		position(t),
		orientation(r) { orientation.normalize(); }	
	
	/// Affine transformation from world space to pose coordinate system.
	Eigen::Affine3f transformation_from_world() const;

	/// Affine transformation from pose coordinate system to world space.
	Eigen::Affine3f transformation_to_world() const;
	
	Eigen::Affine3f transformation_from(const pose& ps) const {
		return transformation_from_world() * ps.transformation_to_world();
	}
	
	Eigen::Affine3f transformation_to(const pose& ps) const {
		return ps.transformation_from_world() * transformation_to_world();
	}
		
	Eigen::Vector3f euler_angles(std::ptrdiff_t a0 = 0, std::ptrdiff_t a1 = 1, std::ptrdiff_t a2 = 2) const;
	
	std::string to_string() const;
	static pose from_string(const std::string&);
		
	Eigen::Vector3f transform_from_world(const Eigen::Vector3f& p) const {
		return transformation_from_world() * p;
	}
	
	Eigen::Vector3f transform_to_world(const Eigen::Vector3f& p) const {
		return transformation_to_world() * p;
	}
};


std::ostream& operator<<(std::ostream&, const pose&);

}

#endif
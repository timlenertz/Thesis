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
	/// Position vector.
	Eigen::Vector3f position;
	
	/// Orientation quaternion. Must be kept normalized.
	Eigen::Quaternionf orientation;

	/// Create identity pose.
	pose();
		
	/// Copy-construct pose.
	pose(const pose&) = default;
	
	/// Construct pose from affine transformation.
	/// Assumes that the transformation consists only of translation and rotation.
	pose(const Eigen::Affine3f&);
	
	// Construct pose with given translation and rotation.
	template<typename Translation, typename Rotation>
	pose(const Translation& t, const Rotation& r) :
		position(t),
		orientation(r) { }	
	
	/// Affine transformation from world space to pose coordinate system.
	Eigen::Affine3f transformation_from_world() const;

	/// Affine transformation from pose coordinate system to world space.
	Eigen::Affine3f transformation_to_world() const;
	
	pose transform(const Eigen::Affine3f&) const;
};

}

#endif
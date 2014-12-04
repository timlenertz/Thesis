#ifndef PCF_POSE_H_
#define PCF_POSE_H_

#include <Eigen/Geometry>

namespace pcf {

/**
Position and orientation in space.
Defines an orthonormal coordinate system. May be in world space, or relative to another pose. Represented using origin position vector and orientation quaternion.
*/
class pose {
public:
	Eigen::Vector3f position;
	Eigen::Quaternionf orientation;

	/**
	Create identify pose.
	*/
	pose();
	
	pose(const pose&) = default;
	
	template<typename Translation, typename Rotation>
	pose(const Translation& t, const Rotation& r) :
		position(t), orientation(r) { }
	
	/**
	Affine transformation into coordinate system of this pose.
	*/
	Eigen::Affine3f view_transformation() const;
};

}

#endif
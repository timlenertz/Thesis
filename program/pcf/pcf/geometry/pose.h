#ifndef PCF_POSE_H_
#define PCF_POSE_H_

#include <Eigen/Geometry>

namespace pcf {

/**
Position and orientation in space.
*/
class pose {
public:
	Eigen::Vector3f position;
	Eigen::Quaternionf orientation;


	pose();
	pose(const pose&) = default;
	template<typename Translation, typename Rotation> pose(const Translation& t, const Rotation& r) :
		position(t), orientation(r) { }
	
	/**
	Affine transformation into view space at this pose.
	*/
	
	Eigen::Affine3f view_transformation() const;
	
	/**
	Homogeneous 4x4 transformation matrix.
	*/
	Eigen::Matrix4f view_matrix() const;
};

}

#endif
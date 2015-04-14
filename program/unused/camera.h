#ifndef PCF_CAMERA_H_
#define PCF_CAMERA_H_

#include <Eigen/Geometry>
#include <array>
#include "angle.h"
#include "pose.h"
#include "spherical_coordinates.h"
#include "projection_frustum.h"

namespace pcf {

/**
Camera at a given pose in space and with perspective projection parameters.

*/
class camera {
protected:
	pose pose_; ///< Pose of camera.
	projection_frustum frustum_; ///< Frustum, without view transformation.
	
public:
	camera(const pose&, const projection_frustum&);
	camera(const camera&) = default;
		
	/// Transformation from world space to camera view space.
	Eigen::Affine3f view_transformation() const;
	
	/// Transformation from camera view space to projection space.
	Eigen::Projective3f projection_transformation() const;
	Eigen::Projective3f view_projection_transformation() const;
	
	/// Camera viewing frustum, set in camera view space.
	const projection_frustum& relative_viewing_frustum() const;

	/// Camera viewing frustum, set in world space, i.e. using camera pose.
	frustum viewing_frustum() const;
	
	const pose& camera_pose() const;

	float depth_sq(const Eigen::Vector3f&) const;
	float depth(const Eigen::Vector3f&) const;	
	bool in_field_of_view(const Eigen::Vector3f&) const;
	
	spherical_coordinates to_spherical(const Eigen::Vector3f&) const;
	Eigen::Vector3f point(const spherical_coordinates&) const;
};

}

#endif

#ifndef PCF_CAMERA_H_
#define PCF_CAMERA_H_

#include <Eigen/Geometry>
#include <array>
#include "angle.h"
#include "pose.h"
#include "spherical_coordinates.h"

namespace pcf {

/**
Camera at a given pose in space and with perspective projection parameters.
*/
class camera {
public:	
	/**
	Coordinates of a point on image plane after perspective projection.
	When point in field of view of camera, value is in [-1, 1]. Third coordinate is depth
	coordinate after projection.
	*/
	using projected_coordinates = Eigen::Vector3f;

private:
	static const float default_z_near_;
	static const float default_z_far_;

	pose pose_;
	angle fov_x_;
	angle fov_y_;	
	
	Eigen::Affine3f view_;
	Eigen::Affine3f view_inv_;
	Eigen::Projective3f view_projection_;
	Eigen::Projective3f view_projection_inv_;

public:
	camera(const pose&, angle fov_x, angle fov_y, float znear = default_z_near_, float zfar = default_z_far_);
	camera(const camera&) = default;
	
	angle field_of_view_x() const { return fov_x_; }
	angle field_of_view_y() const { return fov_y_; }
	
	float distance_sq(const Eigen::Vector3f&) const;
	float distance(const Eigen::Vector3f&) const;
	bool in_field_of_view(const Eigen::Vector3f&) const;
	
	spherical_coordinates to_spherical(const Eigen::Vector3f&) const;
	projected_coordinates to_projected(const Eigen::Vector3f&) const;
	
	Eigen::Vector3f point(const spherical_coordinates&) const;
	Eigen::Vector3f point(const projected_coordinates&) const;	
};

}

#endif

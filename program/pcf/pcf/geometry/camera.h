#ifndef PCF_CAMERA_H_
#define PCF_CAMERA_H_

#include <Eigen/Geometry>
#include <array>
#include "angle.h"
#include "pose.h"
#include "spherical_coordinates.h"

namespace pcf {

struct frustum;

/**
Camera at a given pose in space and with perspective projection parameters.
Handles conversion to and from spherican coordinates.
*/
class camera {
protected:
	static const float default_z_near_;
	static const float default_z_far_;

	pose pose_;
	angle fov_x_;
	angle fov_y_;
	float near_z_;
	float far_z_;
	
	Eigen::Affine3f view_;
	Eigen::Affine3f view_inv_;
	Eigen::Projective3f view_projection_;
	Eigen::Projective3f view_projection_inv_;

	void compute_transformations_();
	
	explicit camera(const pose&);

public:
	camera(const pose&, angle fov_x, angle fov_y, float znear = default_z_near_, float zfar = default_z_far_);
	camera(const camera&) = default;
	
	const pose& get_pose() const { return pose_; }
	void set_pose(const pose&);
	
	angle field_of_view_x() const { return fov_x_; }
	angle field_of_view_y() const { return fov_y_; }
		
	void set_field_of_view(angle fov_x, angle fov_y);
			
	const Eigen::Affine3f& view_transformation() const { return view_; }
	const Eigen::Affine3f& view_transformation_inverse() const { return view_inv_; }
	const Eigen::Projective3f& view_projection_transformation() const { return view_projection_; }
	const Eigen::Projective3f& view_projection_transformation_inverse() const { return view_projection_inv_; }
	
	frustum viewing_frustum() const;
	
	float distance_sq(const Eigen::Vector3f&) const;
	float distance(const Eigen::Vector3f&) const;
	bool in_field_of_view(const Eigen::Vector3f&, bool consider_z_planes = false) const;
	
	spherical_coordinates to_spherical(const Eigen::Vector3f&) const;
	Eigen::Vector3f point(const spherical_coordinates&) const;
};

}

#endif

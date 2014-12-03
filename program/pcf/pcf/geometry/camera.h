#ifndef PCF_CAMERA_H_
#define PCF_CAMERA_H_

#include <Eigen/Geometry>
#include <array>
#include "angle.h"
#include "pose.h"
#include "spherical_coordinates.h"
#include "frustum.h"

namespace pcf {

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
	
	frustum viewing_frustum_;

	void compute_frustum_();
	
	camera(const pose&, angle fov_x, angle fov_y, float znear, float zfar, bool compute);

public:
	camera(const pose& ps, angle fov_x, angle fov_y, float znear = default_z_near_, float zfar = default_z_far_) :
		camera(ps, fov_x, fov_y, znear, zfar, true) { }
		
	camera(const camera&) = default;
	
	const pose& get_pose() const { return pose_; }
	void set_pose(const pose&);
	
	angle field_of_view_x() const { return fov_x_; }
	angle field_of_view_y() const { return fov_y_; }
		
	void set_field_of_view(angle fov_x, angle fov_y);
			
	Eigen::Affine3f view_transformation() const;
	Eigen::Affine3f view_transformation_inverse() const;
	Eigen::Projective3f view_projection_transformation() const;
	Eigen::Projective3f view_projection_transformation_inverse() const;
	
	const frustum& viewing_frustum() const;

	float distance_sq(const Eigen::Vector3f&) const;
	float distance(const Eigen::Vector3f&) const;
	bool in_field_of_view(const Eigen::Vector3f&, bool consider_z_planes = false) const;
	
	spherical_coordinates to_spherical(const Eigen::Vector3f&) const;
	Eigen::Vector3f point(const spherical_coordinates&) const;
};

}

#endif

#ifndef PCF_CAMERA_H_
#define PCF_CAMERA_H_

#include <Eigen/Geometry>
#include <array>
#include "../geometry/angle.h"
#include "../geometry/pose.h"
#include "../geometry/spherical_coordinates.h"
#include "../geometry/projection_frustum.h"

namespace pcf {

/**
Abstract base class for camera.

*/
class camera {
protected:
	pose pose_; ///< Pose of camera.

	explicit camera(const pose&);

public:
	virtual ~camera() = default;

	virtual angle field_of_view_width() const;
	virtual angle field_of_view_height() const;
	virtual std::array<angle, 2> field_of_view_limits_x() const = 0;
	virtual std::array<angle, 2> field_of_view_limits_y() const = 0;
	virtual bool in_field_of_view(const Eigen::Vector3f&) const = 0;

	virtual bool has_viewing_frustum() const = 0;
	virtual projection_frustum relative_viewing_frustum() const = 0;	
	frustum viewing_frustum() const;

	virtual Eigen::Projective3f projection_transformation() const = 0;
	Eigen::Affine3f view_transformation() const;
	Eigen::Projective3f view_projection_transformation() const;
			
	const pose& camera_pose() const;

	float depth_sq(const Eigen::Vector3f&) const;
	float depth(const Eigen::Vector3f&) const;	

	spherical_coordinates to_spherical(const Eigen::Vector3f&) const;
	Eigen::Vector3f point(const spherical_coordinates&) const;
};

}

#endif

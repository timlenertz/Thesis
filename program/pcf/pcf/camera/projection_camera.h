#ifndef PCF_PROJECTION_CAMERA_H_
#define PCF_PROJECTION_CAMERA_H_

#include "camera.h"

namespace pcf {

/**
Camera representing projection to a planar image space.
Represented using its viewing frustum matrix. Can not have fields of view larger than 180 deg.
*/
class projection_camera : public camera {
protected:
	projection_frustum frustum_;

	static angle angle_between_(const Eigen::Vector3f&, const Eigen::Vector3f&);
	static angle angle_between_(const Eigen::Vector4f&, const Eigen::Vector4f&);

public:
	projection_camera(const pose&, const projection_frustum&);
	projection_camera(const camera&);

	angle field_of_view_width() const override;
	angle field_of_view_height() const override;
	std::array<angle, 2> field_of_view_limits_x() const override;
	std::array<angle, 2> field_of_view_limits_y() const override;
	bool in_field_of_view(const Eigen::Vector3f&) const override;

	bool has_viewing_frustum() const override;
	projection_frustum relative_viewing_frustum() const override;
	void set_relative_viewing_frustum(const projection_frustum&);
	
	Eigen::Projective3f projection_transformation() const override;

	float projected_depth(const Eigen::Vector3f&) const;
	Eigen::Vector2f to_projected(const Eigen::Vector3f&) const;
	Eigen::Vector3f point(const Eigen::Vector2f&, float z) const;
};

}

#endif
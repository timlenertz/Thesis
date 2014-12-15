#ifndef PCF_PROJECTION_FRUSTUM_H_
#define PCF_PROJECTION_FRUSTUM_H_

#include "frustum.h"
#include "angle.h"
#include <array>
#include <Eigen/Eigen>

namespace pcf {

/**
Frustum without view transformation.
Corresponds to projection matrix only.
*/
class projection_frustum : public frustum {
private:
	static const float default_z_near_;
	static const float default_z_far_;

protected:
	projection_frustum() = default;
	projection_frustum(const Eigen::Matrix4f&);

public:
	static projection_frustum symmetric_perspective(float near_width, float near_height, float near_z = default_z_near_, float far_z = default_z_far_);
	static projection_frustum symmetric_perspective_fov(angle near_width, angle near_height, float near_z = default_z_near_, float far_z = default_z_far_);
	static projection_frustum symmetric_perspective_fov_x(angle near_width, float aspect_ratio, float near_z = default_z_near_, float far_z = default_z_far_);
	static projection_frustum symmetric_perspective_fov_y(angle near_height, float aspect_ratio, float near_z = default_z_near_, float far_z = default_z_far_);
	static projection_frustum asymmetric_perspective(std::array<float, 2> near_x, std::array<float, 2> near_y, float near_z = default_z_near_, float far_z = default_z_far_);
	static projection_frustum asymmetric_perspective_fov(std::array<angle, 2> near_x, std::array<angle, 2> near_y, float near_z = default_z_near_, float far_z = default_z_far_);
	
	float aspect_ratio() const;
};

}

#endif

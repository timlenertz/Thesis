#ifndef PCF_PROJECTION_BOUNDING_BOX_H_
#define PCF_PROJECTION_BOUNDING_BOX_H_

#include <Eigen/Eigen>
#include "angle.h"
#include "bounding_box.h"

namespace pcf {

class projection_bounding_box : public bounding_box {
private:
	static const float default_z_near_;
	static const float default_z_far_;

public:
	static projection_bounding_box symmetric_orthogonal(float width, float height, float near_z = default_z_near_, float far_z = default_z_far_);
	static projection_bounding_box symmetric_orthogonal_x(float width, float aspect_ratio, float near_z = default_z_near_, float far_z = default_z_far_);
	static projection_bounding_box symmetric_orthogonal_y(float height, float aspect_ratio, float near_z = default_z_near_, float far_z = default_z_far_);
	static projection_bounding_box asymmetric_orthogonal(std::array<float, 2> x, std::array<float, 2> y, float near_z = default_z_near_, float far_z = default_z_far_);
	
	projection_bounding_box(std::array<float, 2> x, std::array<float, 2> y, float near_z, float far_z);

	Eigen::Matrix4f orthogonal_projection_matrix() const;
	
	float aspect_ratio() const;
	bool is_symmetric_x() const;
	bool is_symmetric_y() const;
	bool is_symmetric() const;
	
	float width() const;
	float height() const;
	float near_z() const;
	float far_z() const;
};

}

#endif

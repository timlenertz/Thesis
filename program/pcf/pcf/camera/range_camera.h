#ifndef PCF_RANGE_CAMERA_H_
#define PCF_RANGE_CAMERA_H_

#include "camera.h"
#include <array>

namespace pcf {

class range_camera : public camera {
private:
	std::array<angle, 2> azimuth_limits_;
	std::array<angle, 2> elevation_limits_;
	
public:
	angle field_of_view_width() const override;
	angle field_of_view_height() const override;
	std::array<angle, 2> field_of_view_limits_x() const override;
	std::array<angle, 2> field_of_view_limits_y() const override;
	bool in_field_of_view(const Eigen::Vector3f&) const override;

	bool has_viewing_frustum() const override;
	projection_frustum relative_viewing_frustum() const override;
	
	Eigen::Projective3f projection_transformation() const override;	
};

}

#endif

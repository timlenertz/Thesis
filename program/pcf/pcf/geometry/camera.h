#ifndef PCF_CAMERA_H_
#define PCF_CAMERA_H_

#include <Eigen/Geometry>
#include <array>
#include "angle.h"
#include "pose.h"

namespace pcf {

class camera {
public:
	using spherical_coordinates = std::array<angle, 3>;
	
private:
	pose pose_;
	float fov_x_;
	float fov_y_;	
	
	Eigen::Affine3f view_;

	void update_();

public:
	camera(const pose& p, float fov, float aspect_ratio);
	camera(const camera&) = default;
	
	spherical_coordinates to_spherical(const Eigen::Vector3f&);
	Eigen::Vector3f from_spherical(const spherical_coordinates&);
};

}

#endif

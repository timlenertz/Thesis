#ifndef PCF_FRUSTUM_H_
#define PCF_FRUSTUM_H_

#include "plane.h"
#include <Eigen/Eigen>
#include <array>

namespace pcf {

class bounding_box;

struct frustum {
	enum intersection {
		outside_frustum = 0,
		inside_frustum,
		partially_inside_frustum
	};
	
	enum {
		near_plane = 0,
		far_plane,
		left_plane,
		right_plane,
		bottom_plane,
		top_plane
	};
	
	std::array<plane, 6> planes;
	
	frustum() = default;
	frustum(const frustum&) = default;
	explicit frustum(const Eigen::Matrix4f& mvp);
	
	float far_plane_distance() const;
	float near_plane_distance() const;
	
	bool contains(const Eigen::Vector3f&) const;
	intersection contains(const bounding_box&) const;
};

}

#endif
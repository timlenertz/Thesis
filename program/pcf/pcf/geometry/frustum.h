#ifndef PCF_FRUSTUM_H_
#define PCF_FRUSTUM_H_

#include "plane.h"
#include <Eigen/Eigen>
#include <array>

namespace pcf {

struct bounding_box;

/**
Pyramid frustum, clipped by near and far planes.
Used to represent viewing frustum of a camera. Stored using its 6 planes, can be generated from view-projection matrix. Provides test for intersection with bounding box.
*/
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
			
	bool contains(const Eigen::Vector3f&) const;
	intersection contains(const bounding_box&) const;
};

}

#endif
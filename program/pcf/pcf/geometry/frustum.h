#ifndef PCF_FRUSTUM_H_
#define PCF_FRUSTUM_H_

#include "plane.h"
#include <Eigen/Eigen>
#include <Eigen/Geometry>
#include <array>
#include <utility>

namespace pcf {

struct bounding_box;

using frustum_planes = std::array<plane, 6>;

/**
Pyramid frustum, clipped by near and far planes.
Represented using the view projection matrix. Provides extraction of the 6 planes, the corners and edges, and intersection test with bounding box.
*/
struct frustum {
	enum intersection {
		outside_frustum = 0,
		inside_frustum,
		partially_inside_frustum
	};
	
	using edge = std::pair<Eigen::Vector3f, Eigen::Vector3f>;
	
	Eigen::Matrix4f view_projection_matrix;

	plane near_plane() const;
	plane far_plane() const;
	plane left_plane() const;
	plane right_plane() const;
	plane bottom_plane() const;
	plane top_plane() const;
	
	frustum_planes planes() const;
	std::array<Eigen::Vector3f, 8> corners() const;
	std::array<edge, 12> edges() const;
		
	frustum() = default;
	frustum(const frustum&) = default;
	explicit frustum(const Eigen::Matrix4f& mvp);
				
	bool contains(const Eigen::Vector3f&, bool consider_z_planes = true) const;
	intersection contains(const bounding_box&) const;
	static intersection contains(const frustum_planes&, const bounding_box&);
};

}

#endif
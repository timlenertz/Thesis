#include "frustum.h"
#include "bounding_box.h"
#include <iostream>

namespace pcf {


frustum::frustum(const Eigen::Matrix4f& m) :
	view_projection_matrix(m) { }


plane frustum::near_plane() const {
	const Eigen::Matrix4f& m = view_projection_matrix.transpose();
	return plane(m(0,3) + m(0,2), m(1,3) + m(1,2), m(2,3) + m(2,2), m(3,3) + m(3,2));
}

plane frustum::far_plane() const {
	const Eigen::Matrix4f& m = view_projection_matrix.transpose();
	return plane(m(0,3) - m(0,2), m(1,3) - m(1,2), m(2,3) - m(2,2), m(3,3) - m(3,2));
}

plane frustum::left_plane() const {
	const Eigen::Matrix4f& m = view_projection_matrix.transpose();
	return plane(m(0,3) + m(0,0), m(1,3) + m(1,0), m(2,3) + m(2,0), m(3,3) + m(3,0));
}

plane frustum::right_plane() const {
	const Eigen::Matrix4f& m = view_projection_matrix.transpose();
	return plane(m(0,3) - m(0,0), m(1,3) - m(1,0), m(2,3) - m(2,0), m(3,3) - m(3,0));
}

plane frustum::bottom_plane() const {
	const Eigen::Matrix4f& m = view_projection_matrix.transpose();
	return plane(m(0,3) + m(0,1), m(1,3) + m(1,1), m(2,3) + m(2,1), m(3,3) + m(3,1));
}

plane frustum::top_plane() const {
	const Eigen::Matrix4f& m = view_projection_matrix.transpose();
	return plane(m(0,3) - m(0,1), m(1,3) - m(1,1), m(2,3) - m(2,1), m(3,3) - m(3,1));
}


frustum_planes frustum::planes() const {
	return {
		near_plane(),
		far_plane(),
		left_plane(),
		right_plane(),
		bottom_plane(),
		top_plane()
	};
}


std::array<Eigen::Vector3f, 8> frustum::corners() const {
	Eigen::Matrix4f view_projection_inv = view_projection_matrix.inverse();
	
	std::array<Eigen::Vector3f, 8> corn {
		Eigen::Vector3f(-1, -1, -1),
		Eigen::Vector3f(-1, +1, -1),
		Eigen::Vector3f(-1, +1, +1),
		Eigen::Vector3f(-1, -1, +1),
		Eigen::Vector3f(+1, -1, -1),
		Eigen::Vector3f(+1, +1, -1),
		Eigen::Vector3f(+1, +1, +1),
		Eigen::Vector3f(+1, -1, +1),
	};
	
	for(Eigen::Vector3f& p : corn) {
		Eigen::Vector4f world_point = view_projection_inv * p.homogeneous();
		p = (world_point / world_point[3]).head(3);
	}
	
	return corn;
}


std::array<frustum::edge, 12> frustum::edges() const {
	auto corn = corners();
	return {
		// Near Plane
		edge(corn[0], corn[1]),
		edge(corn[1], corn[2]),
		edge(corn[2], corn[3]),
		edge(corn[3], corn[0]),
		
		// Far Plane
		edge(corn[0+4], corn[1+4]),
		edge(corn[1+4], corn[2+4]),
		edge(corn[2+4], corn[3+4]),
		edge(corn[3+4], corn[0+4]),
		
		// Connecting
		edge(corn[0], corn[0+4]),
		edge(corn[1], corn[1+4]),
		edge(corn[2], corn[2+4]),
		edge(corn[3], corn[3+4])
	};
}




bool frustum::contains(const Eigen::Vector3f& world_point, bool consider_z_planes) const {
	Eigen::Vector4f projected_point = view_projection_matrix * world_point.homogeneous();
	projected_point /= projected_point[3];
	if(projected_point[0] < -1 || projected_point[0] > 1) return false;
	if(projected_point[1] < -1 || projected_point[1] > 1) return false;
	if(consider_z_planes && (projected_point[2] < -1 || projected_point[2] > 1)) return false;
	return true;
}


frustum::intersection frustum::contains(const bounding_box& box) const {
	return contains(planes(), box);
}


frustum::intersection frustum::contains(const frustum_planes& fr_planes, const bounding_box& box) {
	const Eigen::Vector3f& a = box.origin;
	const Eigen::Vector3f& b = box.extremity;
	std::size_t c, c2 = 0;
	for(const plane& p : fr_planes) {
		c = 0;
		if(p.normal[0]*a[0] + p.normal[1]*a[1] + p.normal[2]*a[2] + p.distance > 0) ++c;
		if(p.normal[0]*b[0] + p.normal[1]*a[1] + p.normal[2]*a[2] + p.distance > 0) ++c;
		if(p.normal[0]*a[0] + p.normal[1]*b[1] + p.normal[2]*a[2] + p.distance > 0) ++c;
		if(p.normal[0]*b[0] + p.normal[1]*b[1] + p.normal[2]*a[2] + p.distance > 0) ++c;
		if(p.normal[0]*a[0] + p.normal[1]*a[1] + p.normal[2]*b[2] + p.distance > 0) ++c;
		if(p.normal[0]*b[0] + p.normal[1]*a[1] + p.normal[2]*b[2] + p.distance > 0) ++c;
		if(p.normal[0]*a[0] + p.normal[1]*b[1] + p.normal[2]*b[2] + p.distance > 0) ++c;
		if(p.normal[0]*b[0] + p.normal[1]*b[1] + p.normal[2]*b[2] + p.distance > 0) ++c;
		if(c == 0) return outside_frustum;
		if(c == 8) ++c2;
	}
	return (c2 == 6) ? inside_frustum : partially_inside_frustum;
}


}
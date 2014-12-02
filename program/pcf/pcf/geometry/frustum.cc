#include "frustum.h"
#include "bounding_box.h"
#include <iostream>

namespace pcf {


frustum::frustum(const Eigen::Matrix4f& m) :
planes ({
	plane(m(0,3) + m(0,2), m(1,3) + m(1,2), m(2,3) + m(2,2), m(3,3) + m(3,2)), // near_plane
	plane(m(0,3) - m(0,2), m(1,3) - m(1,2), m(2,3) - m(2,2), m(3,3) - m(3,2)), // far_plane
	plane(m(0,3) + m(0,0), m(1,3) + m(1,0), m(2,3) + m(2,0), m(3,3) + m(3,0)), // left_plane
	plane(m(0,3) - m(0,0), m(1,3) - m(1,0), m(2,3) - m(2,0), m(3,3) - m(3,0)), // right_plane
	plane(m(0,3) + m(0,1), m(1,3) + m(1,1), m(2,3) + m(2,1), m(3,3) + m(3,1)), // bottom_plane
	plane(m(0,3) - m(0,1), m(1,3) - m(1,1), m(2,3) - m(2,1), m(3,3) - m(3,1))  // top_plane
}) {
	std::cout << "----" << std::endl;
	std::ptrdiff_t i = 0;
	std::cout << "near_plane: d=" << planes[i].distance << " ; normal=(" << planes[i].normal[0] << ", " << planes[i].normal[1] << ", " << planes[i++].normal[2] << ")" << std::endl;
	std::cout << "far_plane: d=" << planes[i].distance << " ; normal=(" << planes[i].normal[0] << ", " << planes[i].normal[1] << ", " << planes[i++].normal[2] << ")" << std::endl;
	std::cout << "left_plane: d=" << planes[i].distance << " ; normal=(" << planes[i].normal[0] << ", " << planes[i].normal[1] << ", " << planes[i++].normal[2] << ")" << std::endl;
	std::cout << "right_plane: d=" << planes[i].distance << " ; normal=(" << planes[i].normal[0] << ", " << planes[i].normal[1] << ", " << planes[i++].normal[2] << ")" << std::endl;
	std::cout << "bottom_plane: d=" << planes[i].distance << " ; normal=(" << planes[i].normal[0] << ", " << planes[i].normal[1] << ", " << planes[i++].normal[2] << ")" << std::endl;
	std::cout << "top_plane: d=" << planes[i].distance << " ; normal=(" << planes[i].normal[0] << ", " << planes[i].normal[1] << ", " << planes[i++].normal[2] << ")" << std::endl;
}


bool frustum::contains(const Eigen::Vector3f& pt) const {
	for(const plane& pl : planes) if(signed_distance(pt, pl) < 0) return false;
	return true;
}



frustum::intersection frustum::contains(const bounding_box& box) const {
	const Eigen::Vector3f& a = box.origin;
	const Eigen::Vector3f& b = box.extremity;
	std::size_t c, c2 = 0;
	for(const plane& p : planes) {
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
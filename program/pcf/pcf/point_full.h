#ifndef PCF_POINT_FULL_H_
#define PCF_POINT_FULL_H_

#include "point.h"
#include <utility>

namespace pcf {

/**
Point of a cloud, consisting of X, Y, Z coordinates, RGB color information and normal vector.
*/
class ALIGNAS(32) point_full : public point_xyz {
public:
	static const rgb_color default_color;

	Eigen::Vector3f normal;
	rgb_color color;
	std::uint8_t whatever;
	
	using point_xyz::point_xyz;
	using point_xyz::operator=;
	
	point_full() = default;
	point_full(const point_xyz& pt) : point_xyz(pt), normal(Eigen::Vector3f::Zero()), color(default_color) { }

	point_full(float x, float y, float z, std::uint8_t r, std::uint8_t g, std::uint8_t b) :
	point_xyz(x, y, z), color(r, g, b) { }
};

}


namespace std {

template<>
inline void swap(pcf::point_full& a, pcf::point_full& b) {
	a.homogeneous_coordinates.swap(b.homogeneous_coordinates);
	a.normal.swap(b.normal);
	std::swap(a.color, b.color);
}

}

#endif
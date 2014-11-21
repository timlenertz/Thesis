#ifndef PCF_POINT_FULL_H_
#define PCF_POINT_FULL_H_

#include "point.h"

namespace pcf {

/**
Point of a cloud, consisting of X, Y, Z coordinates, RGB color information and normal vector.
*/
class ALIGNAS(32) point_full : public point_xyz {
public:
	Eigen::Vector3f normal;
	rgb_color color;
	std::uint8_t whatever;
	
	using point_xyz::point_xyz;
	using point_xyz::operator=;
	
	point_full() = default;
	point_full(float x, float y, float z, std::uint8_t r, std::uint8_t g, std::uint8_t b) :
	point_xyz(x, y, z), color(r, g, b) { }
};

}

#endif
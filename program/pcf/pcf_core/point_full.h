#ifndef PCF_POINT_FULL_H_
#define PCF_POINT_FULL_H_

#include "point.h"
#include "rgb_color.h"
#include <utility>

namespace pcf {

/**
Point of a cloud, consisting of X, Y, Z coordinates, RGB color information and normal vector. Due to the point_xyz base class it does not quality as C++ POD type, but is still be used as such in the project. Works correctly in tested compilers.
*/
class ALIGNAS(32) point_full : public point_xyz {
public:
	Eigen::Vector3f normal;
	rgb_color color;
	std::uint8_t weight;
	
	point_full();
	point_full(const point_full&) = default;
	point_full(const point_xyz& pt);
	point_full(float x, float y, float z, std::uint8_t r, std::uint8_t g, std::uint8_t b);
		
	point_full& operator=(const point_full&) = default;
	point_full& operator=(const point_xyz&);
	
	const rgb_color& get_color() const { return color; }
	void set_color(const rgb_color& col) { color = col; }
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
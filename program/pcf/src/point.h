#ifndef PCF_POINT_H_
#define PCF_POINT_H_

#include <Eigen/Eigen>
#include <cstdint>

namespace pcf {

using point_xyz = Eigen::Vector3f;

struct point_xyzrgb : public point_xyz {
	using point_xyz::point_xyz;
	
	point_xyzrgb() = default;
	point_xyzrgb(float x, float y, float z, std::uint8_t r, std::uint8_t g, std::uint8_t b):
		point_xyz(x, y, z), color{r, g, b} { }

	std::uint8_t color[3] = { 255, 255, 255 };
};

}

#endif
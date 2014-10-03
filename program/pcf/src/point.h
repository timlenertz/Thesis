#ifndef PCF_POINT_H_
#define PCF_POINT_H_

#include <Eigen/Eigen>
#include <cstdint>

#include "rgb_color.h"

namespace pcf {

struct alignas(16) point_xyz : public Eigen::Vector3f {
	using Eigen::Vector3f::Vector3f;
};

struct point_xyzrgb : public point_xyz {
	rgb_color color;

	using point_xyz::point_xyz;

	point_xyzrgb() = default;
	
	point_xyzrgb(float x, float y, float z, std::uint8_t r, std::uint8_t g, std::uint8_t b) :
		point_xyz(x, y, z), color(r, g, b) { }

	template<typename OtherDerived>
	point_xyzrgb(const Eigen::MatrixBase<OtherDerived>& m) : point_xyz(m) { }
	
	template<typename OtherDerived>
	point_xyzrgb& operator=(const Eigen::MatrixBase<OtherDerived>& m) {
		this->point_xyz::operator=(m); return *this;
	}
};

}

#endif
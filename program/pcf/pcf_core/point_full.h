#ifndef PCF_POINT_FULL_H_
#define PCF_POINT_FULL_H_

#include "point.h"
#include "rgb_color.h"
#include <utility>
#include <cmath>
#include <cstdint>

namespace pcf {

/**
Point of a cloud, consisting of X, Y, Z coordinates, RGB color information, normal vector and weight.

*/
class ALIGNAS(64) point_full : public point_xyz {
public:
	Eigen::Vector3f normal = Eigen::Vector3f::Zero();
	float weight = NAN;
	std::uint32_t index = 0;
	rgb_color color = default_color;
	char pad[25];
	
public:
	point_full() :
		point_xyz() {}

	point_full(const point_xyz& pt) :
		point_xyz(pt) { }

	point_full(const Eigen::Vector3f& v) :
		point_xyz(v) { }

	point_full(float x, float y, float z) :
		point_xyz(x, y, z) { }

	point_full(float x, float y, float z, const rgb_color& col) :
		point_xyz(x, y, z), color(col) { }
		
	point_full(const point_full&) = default;


	point_full& operator=(const point_full&) = default;
	point_full& operator=(const point_xyz&);
	point_full& operator=(const Eigen::Vector3f& v) {
		return operator=(point_xyz(v));
	}

	template<typename Transform>
	void apply_transformation(const Transform& t) {
		homogeneous_coordinates = t * homogeneous_coordinates;
		normal = t * normal;
	}

	const rgb_color& get_color() const { return color; }
	void set_color(const rgb_color& col) { color = col; }
	
	Eigen::Vector3f get_normal(bool normalized = true) const {
		return normalized ? normal.normalized() : normal;
	}
	void set_normal(const Eigen::Vector3f& n) { normal = n; }
	bool has_normal() const { return normal.isZero(); }
	void flip_normal() { normal = -normal; }
	
	float get_weight() const { return weight; }
	void set_weight(float w) { weight = w; }
	bool has_weight() const { return ! isnan(weight); }
	
	std::ptrdiff_t get_index() const { return (std::ptrdiff_t)index; }
	void set_index(std::ptrdiff_t i) { index = i; }

	void swap(point_full&);

	friend std::ostream& operator<<(std::ostream&, const point_full&);
};

static_assert(sizeof(point_full) == 64, "point_full has to be 64 byte long.");

}


namespace std {

template<>
inline void swap(pcf::point_full& a, pcf::point_full& b) {
	a.swap(b);
}

}

#endif
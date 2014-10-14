#ifndef PCF_POINT_H_
#define PCF_POINT_H_

#include <Eigen/Eigen>
#include <ostream>
#include <cstdint>
#include <cmath>
#include <cassert>

#include "rgb_color.h"

namespace pcf {

struct alignas(16) point_xyz {
	Eigen::Vector4f homogeneous_coordinates;
	// last component needs to be 1
	// otherwise point is invalid

	/// Default constructor. Initializes invalid point.
	point_xyz() : homogeneous_coordinates(Eigen::Vector4f::Zero()) { }
	
	point_xyz(float x, float y, float z) : homogeneous_coordinates(x, y ,z, 1) { }
	point_xyz(const Eigen::Vector3f& v) : homogeneous_coordinates(v[0], v[1], v[2], 1) { }
	
	point_xyz& operator=(const point_xyz&) = default;
	point_xyz& operator=(const Eigen::Vector3f& v) {
		homogeneous_coordinates[0] = v[0];
		homogeneous_coordinates[1] = v[1];
		homogeneous_coordinates[2] = v[2];
		homogeneous_coordinates[3] = 1;
		return *this;
	}
	
	point_xyz& operator=(const Eigen::Vector4f& v) {
		assert(v[3] == 1);
		homogeneous_coordinates = v;
		return *this;
	}
	
	bool valid() const { return (homogeneous_coordinates[3] == 1); }
	explicit operator bool () const { return valid(); }
	void invalidate() { homogeneous_coordinates[3] = 0; }
			
	float* data() { return homogeneous_coordinates.data(); }
	const float* data() const { return homogeneous_coordinates.data(); }
	
	float& operator[](std::ptrdiff_t i) { assert(i >= 0 && i <= 2); return homogeneous_coordinates[i]; }
	float operator[](std::ptrdiff_t i) const { assert(i >= 0 && i <= 2); return homogeneous_coordinates[i]; }
	
	Eigen::Vector3f coordinates() const { return homogeneous_coordinates.block(0, 0, 3, 1); }
	
	template<typename Transform>
	void apply_transformation(const Transform& t) {
		homogeneous_coordinates = t * homogeneous_coordinates;
	}
	
	friend std::ostream& operator<<(std::ostream& str, const point_xyz& p) {
		if(p.valid()) str << '(' << p[0] << ", " << p[1] << ", "<< p[2] << ')';
		else str << "(none)";
		return str;
	}
};


class alignas(32) point_full : public point_xyz {
public:
	Eigen::Vector3f normal;
	rgb_color color;
	std::uint8_t whatever;
	
	using point_xyz::point_xyz;
	using point_xyz::operator=;
	
	point_full(float x, float y, float z, std::uint8_t r, std::uint8_t g, std::uint8_t b) :
	point_xyz(x, y, z), color(r, g, b) { }
};


inline float euclidian_distance_sq(const point_xyz& a, const point_xyz& b) {
	return (b.homogeneous_coordinates - a.homogeneous_coordinates).squaredNorm();
}

inline float euclidian_distance(const point_xyz& a, const point_xyz& b) {
	return std::sqrt(euclidian_distance_sq(a, b));
}


}

#endif
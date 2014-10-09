#ifndef PCF_POINT_H_
#define PCF_POINT_H_

#include <Eigen/Eigen>
#include <ostream>
#include <cstdint>
#include <cmath>
#include <cassert>

#include "rgb_color.h"

namespace pcf {

class alignas(16) point_xyz {
private:
	Eigen::Vector4f homogeneous_;

public:
	point_xyz(float x = 0, float y = 0, float z = 0) : homogeneous_(x, y ,z, 1) { }
	point_xyz(const Eigen::Vector3f& v) : homogeneous_(v[0], v[1], v[2], 1) { }
	point_xyz(const Eigen::Vector4f& v) : homogeneous_(v[0], v[1], v[2], 1) { assert(v[3] == 1); }
	
	point_xyz& operator=(const point_xyz&) = default;
	point_xyz& operator=(const Eigen::Vector3f& v) {
		assert(homogeneous_[3] == 1);
		homogeneous_[0] = v[0]; homogeneous_[1] = v[1]; homogeneous_[2] = v[2];
		return *this;
	}
	
	point_xyz& operator=(const Eigen::Vector4f& v) {
		assert(v[3] == 1);
		homogeneous_ = v;
		return *this;
	}
	
	const Eigen::Vector4f& homogeneous_coordinates() const { return homogeneous_; }
	Eigen::Vector4f& homogeneous_coordinates() { return homogeneous_; }
	
	float* data() { return homogeneous_.data(); }
	const float* data() const { return homogeneous_.data(); }
	
	float& operator[](std::ptrdiff_t i) { assert(i >= 0 && i <= 2); return homogeneous_[i]; }
	float operator[](std::ptrdiff_t i) const { assert(i >= 0 && i <= 2); return homogeneous_[i]; }
		
	template<typename Transform>
	void apply_transformation(const Transform& t) {
		homogeneous_ = t * homogeneous_;
	}
	
	friend std::ostream& operator<<(std::ostream& str, const point_xyz& p) {
		str << '(' << p[0] << ", " << p[1] << ", "<< p[2] << ')';
		return str;
	}
};

inline float euclidian_distance_sq(const point_xyz& a, const point_xyz& b) {
	Eigen::Vector4f d = b.homogeneous_coordinates() - a.homogeneous_coordinates();
	return d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
}

inline float euclidian_distance(const point_xyz& a, const point_xyz& b) {
	return std::sqrt(euclidian_distance_sq(a, b));
}

class alignas(32) point_full : public point_xyz {
public:
	Eigen::Vector3f normal;
	rgb_color color;
	std::uint8_t whatever;
};



}

#endif
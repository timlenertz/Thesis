#ifndef PCF_POINT_H_
#define PCF_POINT_H_

#include <Eigen/Eigen>
#include <ostream>
#include <cstdint>
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
	
	float* data() { return homogeneous_.data(); }
	const float* data() const { return homogeneous_.data(); }
	
	float& operator[](std::ptrdiff_t i) { return homogeneous_[i]; }
	float operator[](std::ptrdiff_t i) const { return homogeneous_[i]; }
	
	template<typename Transform>
	void apply_transformation(const Transform& t) {
		homogeneous_ = t * homogeneous_;
	}
	
	friend std::ostream& operator<<(std::ostream& str, const point_xyz& p) {
		str << '(' << p[0] << ", " << p[1] << ", "<< p[2] << ')';
		return str;
	}
};



}

#endif
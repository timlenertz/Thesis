#ifndef PCF_POINT_XYZ_H_
#define PCF_POINT_XYZ_H_

#include <Eigen/Eigen>
#include <iosfwd>
#include <cstdint>
#include <cmath>
#include <cassert>

#include "rgb_color.h"

namespace pcf {

/**
Point defined only by X, Y, Z coordinates.
Can be marked as invalid. Aligned to 16 byte boundary, homologous to float[4], to allow for SIMD optimizations.
*/
struct alignas(16) point_xyz {
	/// Homogeneous coordinates of point. When last component is not 1, point is invalid. Hence representation is unique.
	Eigen::Vector4f homogeneous_coordinates;


	/// Default constructor. Initializes invalid point.
	point_xyz() : homogeneous_coordinates(Eigen::Vector4f::Zero()) { }
	
	point_xyz(float x, float y, float z) : homogeneous_coordinates(x, y ,z, 1) { }
	point_xyz(const Eigen::Vector3f& v) : homogeneous_coordinates(v[0], v[1], v[2], 1) { }
	
	point_xyz& operator=(const point_xyz&) = default;
	point_xyz& operator=(const Eigen::Vector3f& v) {
		homogeneous_coordinates = Eigen::Vector4f(v[0], v[1], v[2], 1);
		return *this;
	}
	
	bool operator==(const point_xyz& pt) const {
		if(pt.valid() != valid()) return false;
		else if(valid()) return (coordinates() == pt.coordinates());
		else return true;
	}
	
	bool operator!=(const point_xyz& pt) const { return ! operator==(pt); }
	
	bool valid() const { return (homogeneous_coordinates[3] == 1); }
	explicit operator bool () const { return valid(); }
	void invalidate() { homogeneous_coordinates[3] = 0; }
	
	float& operator[](std::ptrdiff_t i) { assert(valid() && i >= 0 && i <= 2); return homogeneous_coordinates[i]; }
	const float& operator[](std::ptrdiff_t i) const { assert(valid() && i >= 0 && i <= 2); return homogeneous_coordinates[i]; }
	
	float* data() { return homogeneous_coordinates.data(); }
	const float* data() const { return homogeneous_coordinates.data(); }
		
	Eigen::Vector3f coordinates() const { return homogeneous_coordinates.block(0, 0, 3, 1); }
	
	template<typename Transform>
	void apply_transformation(const Transform& t) {
		homogeneous_coordinates = t * homogeneous_coordinates;
	}
	
	friend std::ostream& operator<<(std::ostream&, const point_xyz&);
};


inline float euclidian_distance_sq(const point_xyz& a, const point_xyz& b) {
	return (b.homogeneous_coordinates - a.homogeneous_coordinates).squaredNorm();
}

inline float euclidian_distance(const point_xyz& a, const point_xyz& b) {
	return std::sqrt(euclidian_distance_sq(a, b));
}


}

#endif
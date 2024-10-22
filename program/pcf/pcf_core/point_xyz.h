#ifndef PCF_POINT_XYZ_H_
#define PCF_POINT_XYZ_H_

#include <Eigen/Eigen>

#include <iosfwd>
#include <cstdint>
#include <cmath>
#include <cassert>

#include "util/memory.h"
#include "rgb_color.h"

namespace pcf {

/**
Point of a cloud, consisting of X, Y, Z coordinates.
Can be marked as invalid. Aligned to 16 byte boundary, homologous to float[4], to allow for SIMD optimizations. POD type intended for raw memory access. (E.g. could be written into file or OpenGL buffer).
*/
struct ALIGNAS(16) point_xyz {
	/// Homogeneous coordinates of point.
	/// When last component is not 1, point is invalid. All invalid values compare as equal, regardless of first 3 components.
	/// Representation of valid points is unique.
	Eigen::Vector4f homogeneous_coordinates;


	/// Default constructor. Initializes invalid point.
	point_xyz() : homogeneous_coordinates(Eigen::Vector4f::Zero()) { }
	
	point_xyz(float x, float y, float z) : homogeneous_coordinates(x, y, z, 1) { }
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
	
	static const rgb_color default_color;

	const rgb_color& get_color() const { return default_color; }
	void set_color(const rgb_color&) { }
		
	Eigen::Vector3f get_normal() const { return Eigen::Vector3f::Zero(); }
	void set_normal(const Eigen::Vector3f&) { }
	bool has_normal() const { return false; }
	void flip_normal() { }
	
	float get_weight() const { return 1.0; }
	void set_weight(float w) { }
	bool has_weight() const { return false; }

	std::ptrdiff_t get_index() const { return -1; }
	void set_index(std::ptrdiff_t i) { }	

	bool valid() const { return (homogeneous_coordinates[3] == 1); }
	explicit operator bool () const { return valid(); }
	void invalidate() { homogeneous_coordinates[3] = 0; }
	void revalidate() { homogeneous_coordinates[3] = 1; }
		
	float& operator[](std::ptrdiff_t i) { assert(valid() && i >= 0 && i <= 2); return homogeneous_coordinates[i]; }
	const float& operator[](std::ptrdiff_t i) const { assert(valid() && i >= 0 && i <= 2); return homogeneous_coordinates[i]; }
	
	float* data() { return homogeneous_coordinates.data(); }
	const float* data() const { return homogeneous_coordinates.data(); }
		
	Eigen::Vector3f coordinates() const { return homogeneous_coordinates.block(0, 0, 3, 1); }
	operator Eigen::Vector3f () const { return coordinates(); }
	operator Eigen::Vector4f () const { return homogeneous_coordinates; }
	
	template<typename Transform>
	void apply_transformation(const Transform& t) {
		homogeneous_coordinates = t * homogeneous_coordinates;
	}
		
	friend std::ostream& operator<<(std::ostream&, const point_xyz&);
};

}


namespace std {

template<>
inline void swap(pcf::point_xyz& a, pcf::point_xyz& b) {
	// Ensure most efficient swap.
	a.homogeneous_coordinates.swap(b.homogeneous_coordinates);
}

}

#endif
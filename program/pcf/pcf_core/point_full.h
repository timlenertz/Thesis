#ifndef PCF_POINT_FULL_H_
#define PCF_POINT_FULL_H_

#include "point.h"
#include "rgb_color.h"
#include <utility>
#include <cmath>

namespace pcf {

/**
Point of a cloud, consisting of X, Y, Z coordinates, RGB color information, normal vector and weight.
Due to the point_xyz base class it does not quality as C++ POD type, but is still be used as such in the project. Works correctly in tested compilers. Normal vector represents direction only. Weight is encoded as the squared norm of the normal vector. Points cannot have weight 0, and weight read/write less efficient.
*/
class ALIGNAS(32) point_full : public point_xyz {
public:
	Eigen::Vector3f normal = no_normal_();
	rgb_color color = default_color();

private:
	union {
		struct {
			bool has_normal_ : 1;
			bool zero_weight_ : 1;
		};
		std::uint8_t flags_;
	};
	
	static Eigen::Vector3f no_normal_(float w = 1.0) { return Eigen::Vector3f(std::sqrt(w), 0.0, 0.0); }
	
public:
	point_full() :
		point_xyz() { }

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
	point_full& operator=(const Eigen::Vector3f& v) {
		return operator=(point_xyz(v));
	}
	point_full& operator=(const point_xyz&);

	
	const rgb_color& get_color() const { return color; }
	void set_color(const rgb_color& col) { color = col; }
	
	Eigen::Vector3f get_normal(bool normalized = true) const;
	void set_normal(const Eigen::Vector3f&);
	
	float get_weight() const;
	void set_weight(float);

	void swap(point_full&);

	friend std::ostream& operator<<(std::ostream&, const point_full&);
};

static_assert(sizeof(point_full) == 32, "point_full has to be 32 byte long.");

}


namespace std {

template<>
inline void swap(pcf::point_full& a, pcf::point_full& b) {
	a.swap(b);
}

}

#endif
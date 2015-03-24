#ifndef PCF_BOUNDING_BOX_H_
#define PCF_BOUNDING_BOX_H_

#include <iosfwd>
#include <array>
#include <cmath>
#include <utility>
#include <string>
#include <Eigen/Eigen>


namespace pcf {

/**
Axis-aligned 3D cuboid bounding box.
Represented using origin and extremity coordinates.
Point is inside iff for every coordinate i, origin_i <= p_i < extremity_i.
*/
struct bounding_box {
	using edge = std::pair<Eigen::Vector3f, Eigen::Vector3f>;
	using face = std::array<Eigen::Vector3f, 4>;

	Eigen::Vector3f origin;
	Eigen::Vector3f extremity;
	
	bounding_box() = default;
	bounding_box(const bounding_box&) = default;
	bounding_box(const Eigen::Vector3f& o, const Eigen::Vector3f& e);
	
	Eigen::Vector3f center() const;
	Eigen::Vector3f side_lengths() const;
	float side_length(std::ptrdiff_t i = 0) const;
	float volume() const;
	
	void expand_extremity(float ep = 0.1);
	
	bool contains(const Eigen::Vector3f&) const;
	bool contains(const bounding_box&) const;
	
	std::array<Eigen::Vector3f, 8> corners() const;
	std::array<edge, 12> edges() const;
	std::array<face, 6> faces() const;
	
	std::string to_string() const;
	static bounding_box from_string(const std::string&);
		
	friend std::ostream& operator<<(std::ostream&, const bounding_box&);
};



float minimal_distance_sq(const bounding_box& a, const bounding_box& b);
float maximal_distance_sq(const bounding_box& a, const bounding_box& b);

float minimal_distance_sq(const Eigen::Vector3f& p, const bounding_box& b);
float maximal_distance_sq(const Eigen::Vector3f& p, const bounding_box& b);

inline float maximal_distance(const bounding_box& a, const bounding_box& b) {
	return std::sqrt( maximal_distance_sq(a, b) );
}

inline float minimal_distance(const bounding_box& a, const bounding_box& b) {
	return std::sqrt( minimal_distance_sq(a, b) );
}


inline float minimal_distance(const Eigen::Vector3f& p, const bounding_box& b) {
	return std::sqrt( minimal_distance_sq(p, b) );
}

inline float maximal_distance(const Eigen::Vector3f& p, const bounding_box& b) {
	return std::sqrt( maximal_distance_sq(p, b) );
}


}

#endif
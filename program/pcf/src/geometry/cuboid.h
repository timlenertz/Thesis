#ifndef PCF_CUBOID_H_
#define PCF_CUBOID_H_

#include <iosfwd>
#include <array>
#include <Eigen/Eigen>

namespace pcf {

struct cuboid {
	Eigen::Vector3f origin;
	Eigen::Vector3f extremity;
		
	static cuboid infinite();
	
	cuboid() = default;
	cuboid(const cuboid&) = default;
	cuboid(const Eigen::Vector3f& o, const Eigen::Vector3f& e);
	
	Eigen::Vector3f center() const;
	Eigen::Vector3f side_lengths() const;
	float area() const;
	
	bool contains(const Eigen::Vector3f&) const;
	bool contains(const cuboid&) const;
	
	std::array<Eigen::Vector3f, 8> corners() const;
	
	static float minimal_distance_sq(const cuboid& a, const cuboid& b);
	static float maximal_distance_sq(const cuboid& a, const cuboid& b);
	
	friend std::ostream& operator<<(std::ostream&, const cuboid&);
};

}

#endif
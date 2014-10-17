#ifndef PCF_CUBOID_H_
#define PCF_CUBOID_H_

#include <iosfwd>
#include <Eigen/Eigen>

namespace pcf {

struct cuboid {
	Eigen::Vector3f origin;
	Eigen::Vector3f extremity;
		
	static cuboid infinite();
	
	cuboid() = default;
	cuboid(const cuboid&) = default;
	cuboid(const Eigen::Vector3f& o, const Eigen::Vector3f& e) :
		origin(o), extremity(e) { }
	
	Eigen::Vector3f center() const;
	Eigen::Vector3f side_lengths() const;
	float area() const;
	
	bool contains(const Eigen::Vector3f&) const;
	
	friend std::ostream& operator<<(std::ostream&, const cuboid&);
};

}

#endif
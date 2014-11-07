#ifndef PCF_SPHERICAL_COORDINATES_H_
#define PCF_SPHERICAL_COORDINATES_H_

#include "angle.h"
#include <iosfwd>
#include <Eigen/Eigen>

namespace pcf {

/**
Spherical coordinates.
Let O be origin, P be subject point. Radius is distance OP.
Elevation is angle between OP and reference plane.
In cartesian space, reference plane is Y=0. Azimuth is angle between OP and +Z, when projected on plane given by elevation.
*/
class spherical_coordinates {
public:
	float radius;
	angle azimuth;
	angle elevation;
	
	spherical_coordinates(float r, angle az, angle el) :
		radius(r), azimuth(az), elevation(el) { }
	
	static spherical_coordinates from_cartesian(const Eigen::Vector3f&);
	Eigen::Vector3f to_cartesian() const;
	
	bool operator==(const spherical_coordinates&) const;
	bool operator!=(const spherical_coordinates&) const;
};

std::ostream& operator<<(std::ostream&, const spherical_coordinates&);

}

#endif

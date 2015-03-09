#ifndef PCF_SPHERICAL_COORDINATES_H_
#define PCF_SPHERICAL_COORDINATES_H_

#include "angle.h"
#include <iosfwd>
#include <Eigen/Eigen>

namespace pcf {

/**
Spherical coordinates.
Let O be origin, P be subject point. Radius is distance OP.
Azimuth is angle between -Z and the projection of OP on Y=0.
Elevation is (polar) angle between OP and reference plane Y=0. Or equivalently angle between OP and zenith axis +Y, plus pi.
Both azimuth and elevation are in [-pi, pi].
*/
class spherical_coordinates {
public:
	float radius = 0;
	angle azimuth;
	angle elevation;
	
	spherical_coordinates() = default;
	spherical_coordinates(const spherical_coordinates&) = default;
	spherical_coordinates(float r, angle az, angle el) :
		radius(r), azimuth(az), elevation(el) { }
	
	static spherical_coordinates from_cartesian(const Eigen::Vector3f&);
	Eigen::Vector3f to_cartesian() const;
	
	bool operator==(const spherical_coordinates&) const;
	bool operator!=(const spherical_coordinates&) const;
	
	void invalidate() { radius = 0; }
	bool valid() const { return (radius != 0); }
	explicit operator bool () const { return valid(); }
};

std::ostream& operator<<(std::ostream&, const spherical_coordinates&);

}

#endif

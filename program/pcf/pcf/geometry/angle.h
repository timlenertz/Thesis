#ifndef PCF_ANGLE_H_
#define PCF_ANGLE_H_

#include <iosfwd>
#include <cmath>

namespace pcf {

/**
Angle value, internally represented in radiants.
Defines I/O in degrees, and conversion. Provides unambiguity in what angles mean.
TODO: add comparation operators, modulo 2PI operations if needed
*/
class angle {
private:
	static const float rad_per_deg_;

	float radiants_;

public:
	static angle degrees(float d) { return d * rad_per_deg_; }
	static angle radiants(float r) { return r; }	

	angle(float r) : radiants_(r) { }
	angle(const angle&) = default;
	
	float get_radiants() const { return radiants_; }
	float get_degrees() const { return radiants_ / rad_per_deg_; }
	void set_radiants(float r) { radiants_ = r; }
	void set_degrees(float d) { radiants_ = d * rad_per_deg_; }
	
	operator float& () { return radiants_; }
	operator const float& () const { return radiants_; }
	
	angle& operator=(float r) { radiants_ = r; return *this; }
	angle& operator=(const angle&) = default;
	
	bool operator==(angle a) const { return radiants_ == a.radiants_; }
	bool operator!=(angle a) const { return radiants_ != a.radiants_; }
};


std::ostream& operator<<(std::ostream&, angle);
std::istream& operator>>(std::istream&, angle&);

}

#endif

#include "angle.h"
#include <istream>
#include <ostream>
#include <cmath>


namespace pcf {

const float angle::rad_per_deg_ = M_PI / 180.0;

std::ostream& operator<<(std::ostream& str, angle a) {
	str << a.get_degrees() << "°";
	return str;
}

std::istream& operator>>(std::istream& str, angle& a) {
	float deg;
	str >> deg;
	a.set_degrees(deg);
	return str;
}

}
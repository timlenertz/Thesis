#define _USE_MATH_DEFINES
#include <cmath>

#include "angle.h"
#include "math_constants.h"
#include <istream>
#include <ostream>

namespace pcf {

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

angle angle::smallest_between(angle a, angle b) {
	float diff = b - a;
	while(diff < -pi) diff += pi;
	while(diff > +pi) diff -= pi;
	return diff;
}

}
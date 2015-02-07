#include "point_xyz.h"
#include <ostream>

namespace pcf {

std::ostream& operator<<(std::ostream& str, const point_xyz& p) {
	if(p.valid()) str << '(' << p[0] << ", " << p[1] << ", "<< p[2] << ')';
	else str << "(none)";
	return str;
}

}
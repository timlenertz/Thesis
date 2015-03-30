#include "point_full.h"

namespace pcf {

point_full& point_full::operator=(const point_xyz& p) {
	point_xyz::operator=(p);
	normal.setZero();
	weight = NAN;
	index = 0;
	color = default_color;
	return *this;
}

void point_full::swap(point_full& p) {
	std::swap<point_xyz>(*this, p);
	normal.swap(p.normal);
	std::swap(weight, p.weight);
	std::swap(index, p.index);
	std::swap(color, p.color);
}


std::ostream& operator<<(std::ostream& str, const point_full& p) {
	if(p.valid()) {
		str << '(' << p[0] << ", " << p[1] << ", "<< p[2] << ')';
		// TODO
	} else {
		str << "(none)";
	}
	return str;
}

}

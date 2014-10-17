#include "point.h"

namespace pcf {

float euclidian_distance_sq(const point_xyz& a, const point_xyz& b) {
	return (b.homogeneous_coordinates - a.homogeneous_coordinates).squaredNorm();
}

float euclidian_distance(const point_xyz& a, const point_xyz& b) {
	return std::sqrt(euclidian_distance_sq(a, b));
}

void mark_point(point_full& p, std::ptrdiff_t m) {
	m %= 10;
	switch(m) {
		case 0: p.color = 0xff0000; break;
		case 1: p.color = 0xaa0000; break;
		case 2: p.color = 0x0000ff; break;
		case 3: p.color = 0x0000aa; break;
		case 4: p.color = 0x00cc00; break;
		case 5: p.color = 0x006600; break;
		case 6: p.color = 0xff00ff; break;
		case 7: p.color = 0xaa00aa; break;
		case 8: p.color = 0xffcc00; break;
		case 9: p.color = 0xff6600; break;
	}
}

}
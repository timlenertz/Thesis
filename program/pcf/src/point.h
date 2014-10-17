#ifndef PCF_POINT_H_
#define PCF_POINT_H_

#include "point_xyz.h"
#include "point_full.h"

namespace pcf {

inline void mark_point(point_xyz&, std::ptrdiff_t m) { }
void mark_point(point_full&, std::ptrdiff_t m);

float euclidian_distance_sq(const point_xyz&, const point_xyz&);
float euclidian_distance(const point_xyz&, const point_xyz&);

}

#endif
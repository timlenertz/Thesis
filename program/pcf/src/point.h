#ifndef PCF_POINT_H_
#define PCF_POINT_H_

#include "point_xyz.h"
#include "point_full.h"

namespace pcf {

float euclidian_distance_sq(const point_xyz&, const point_xyz&);
float euclidian_distance(const point_xyz&, const point_xyz&);

}

#endif
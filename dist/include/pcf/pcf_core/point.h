#ifndef PCF_POINT_H_
#define PCF_POINT_H_

#include "point_xyz.h"
#include "point_full.h"

namespace pcf {

/**
Compute squared euclidian distance between two points.
Faster than euclidian distance.
*/
float distance_sq(const point_xyz&, const point_xyz&);

/**
Compute euclidian distance between two points.
*/
float distance(const point_xyz&, const point_xyz&);

}

#endif
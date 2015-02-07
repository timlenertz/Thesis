#ifndef PCF_POINT_H_
#define PCF_POINT_H_

#include "point_xyz.h"
#include "point_full.h"

namespace pcf {

float distance_sq(const point_xyz&, const point_xyz&);
float distance(const point_xyz&, const point_xyz&);

}

#endif
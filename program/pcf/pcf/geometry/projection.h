#ifndef PCF_PROJECTION_H_
#define PCF_PROJECTION_H_

#include "angle.h"
#include <Eigen/Geometry>

namespace pcf {

/**
Transformation for perspective projection.
*/
Eigen::Projective3f perspective_projection(angle fov_x, angle fov_y, float znear, float zfar);

}

#endif
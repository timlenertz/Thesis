#ifndef PCF_PROJECTION_H_
#define PCF_PROJECTION_H_

#include "angle.h"
#include <Eigen/Geometry>

namespace pcf {

/**
Transformation for perspective projection.
Takes X and Y field of view angles.
*/
Eigen::Projective3f perspective_projection(angle fov_x, angle fov_y, float znear, float zfar);


/**
Transformation for perspective projection.
Takes X field of view, and aspect radio (w/h) of near (& far) plane.
*/
Eigen::Projective3f perspective_projection_with_aspect_ratio(angle fov_x, float aspect_ratio, float znear, float zfar);

}

#endif
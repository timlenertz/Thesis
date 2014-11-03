#ifndef PCF_PROJECTION_H_
#define PCF_PROJECTION_H_

#include <Eigen/Geometry>

namespace pcf {

Eigen::Projective3f perspective_projection(float fov, float ratio, float znear, float zfar);

}

#endif
#ifndef PCF_PROJECTION_H_
#define PCF_PROJECTION_H_

#include <Eigen/Geometry>

namespace pcf {

Eigen::Matrix4f perspective_projection_matrix(float fov, float ratio, float zfar, float znear);

}

#endif
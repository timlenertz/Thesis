#include "projection.h"
#include <cmath>

namespace pcf {

Eigen::Projective3f perspective_projection(float ratio, float fov, float znear, float zfar) {
	float x_scale = 1.0f / std::tan(fov / 2);
	float y_scale = x_scale / ratio;
	float zdiff = zfar - znear;
	
	Eigen::Matrix4f mat;
	mat <<
		x_scale, 0, 0, 0,
		0, y_scale, 0, 0,
		0, 0, -(zfar + znear)/zdiff, -1,
		0, 0, -2*znear*zfar/zdiff, 0;
		
	return Eigen::Projective3f(mat);
}

}
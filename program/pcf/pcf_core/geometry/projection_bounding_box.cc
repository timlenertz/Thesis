#include "projection_bounding_box.h"

namespace pcf {

const float projection_bounding_box::default_z_near_ = 0.0;
const float projection_bounding_box::default_z_far_ = 1000.0;

projection_bounding_box projection_bounding_box::symmetric_orthogonal(float width, float height, float near_z, float far_z) {
	return projection_bounding_box(
		{ -width/2.0f, +width/2.0f },
		{ +height/2.0f, -height/2.0f },
		near_z, far_z
	);
}


projection_bounding_box projection_bounding_box::symmetric_orthogonal_x(float width, float aspect_ratio, float near_z, float far_z) {
	return projection_bounding_box(
		{ -width/2.0f, +width/2.0f },
		{ +(width / aspect_ratio)/2.0f, -(width / aspect_ratio)/2.0f },
		near_z, far_z
	);
}


projection_bounding_box projection_bounding_box::symmetric_orthogonal_y(float height, float aspect_ratio, float near_z, float far_z) {
	return projection_bounding_box(
		{ -(height * aspect_ratio)/2.0f, +(height * aspect_ratio)/2.0f },
		{ +height/2.0f, -height/2.0f },
		near_z, far_z
	);
}


projection_bounding_box projection_bounding_box::asymmetric_orthogonal(std::array<float, 2> x, std::array<float, 2> y, float near_z, float far_z) {
	return projection_bounding_box(x, y, near_z, far_z);
}


projection_bounding_box::projection_bounding_box(std::array<float, 2> x, std::array<float, 2> y, float near_z, float far_z) :
bounding_box(
	Eigen::Vector3f(x[0], y[0], near_z),
	Eigen::Vector3f(x[1], y[1], far_z)
) {}

Eigen::Matrix4f projection_bounding_box::orthogonal_projection_matrix() const {
	float l = origin[0], r = extremity[0];
	float b = origin[1], t = extremity[1];
	float n = origin[2], f = extremity[2];

	Eigen::Matrix4f mat;
	mat <<
		2.0f/(r - l), 0, 0, -(r + l)/(r - l),
		0, 2.0f/(t - b), 0, -(t + b)/(t - b),
		0, 0, -2.0f/(f - n), -(f + n)/(f - n),
		0, 0, 0, -1;
	return mat;
}




float projection_bounding_box::aspect_ratio() const {
	return width() / height();
}

bool projection_bounding_box::is_symmetric_x() const {
	return (extremity[0] == -origin[0]);
}

bool projection_bounding_box::is_symmetric_y() const {
	return (extremity[1] == -origin[1]);
}

bool projection_bounding_box::is_symmetric() const {
	return is_symmetric_x() && is_symmetric_y();
}

float projection_bounding_box::width() const {
	return extremity[0] - origin[0];
}

float projection_bounding_box::height() const {
	return extremity[1] - origin[1];
}

float projection_bounding_box::near_z() const {
	return origin[2];
}

float projection_bounding_box::far_z() const {
	return extremity[2];
}


}

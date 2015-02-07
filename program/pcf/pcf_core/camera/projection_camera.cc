#include "projection_camera.h"
#include <cmath>

namespace pcf {

projection_camera::projection_camera(const pose& ps, const projection_frustum& fr) :
	camera(ps), frustum_(fr) { }


projection_camera::projection_camera(const camera& cam) :
	camera(cam), frustum_(cam.relative_viewing_frustum()) { }


angle projection_camera::angle_between_(const Eigen::Vector3f& v, const Eigen::Vector3f& u) {
	float dot_product = (v / v.norm()).dot(u / u.norm());
	return std::acos(dot_product);
}


angle projection_camera::angle_between_(const Eigen::Vector4f& v, const Eigen::Vector4f& u) {
	return angle_between_(
		Eigen::Vector3f( (v / v[3]).head(3) ),
		Eigen::Vector3f( (u / u[3]).head(3) )
	);
}


angle projection_camera::field_of_view_width() const {
	Eigen::Projective3f inv_proj = projection_transformation().inverse();
	return angle_between_(
		inv_proj * Eigen::Vector4f(+1, 0, -1, 1),
		inv_proj * Eigen::Vector4f(-1, 0, -1, 1)
	);
}


angle projection_camera::field_of_view_height() const {
	Eigen::Projective3f inv_proj = projection_transformation().inverse();
	return angle_between_(
		inv_proj * Eigen::Vector4f(0, +1, -1, 1),
		inv_proj * Eigen::Vector4f(0, -1, -1, 1)
	);
}


std::array<angle, 2> projection_camera::field_of_view_limits_x() const {
	Eigen::Projective3f inv_proj = projection_transformation().inverse();
	Eigen::Vector4f view_ray = view_ray_direction().homogeneous();
	return {
		angle_between_(inv_proj * Eigen::Vector4f(-1, 0, -1, 1), view_ray),
		angle_between_(inv_proj * Eigen::Vector4f(+1, 0, -1, 1), view_ray)
	};
}

std::array<angle, 2> projection_camera::field_of_view_limits_y() const {
	Eigen::Projective3f inv_proj = projection_transformation().inverse();
	Eigen::Vector4f view_ray = view_ray_direction().homogeneous();
	return {
		angle_between_(inv_proj * Eigen::Vector4f(0, -1, -1, 1), view_ray),
		angle_between_(inv_proj * Eigen::Vector4f(0, +1, -1, 1), view_ray)
	};
}


bool projection_camera::in_field_of_view(const Eigen::Vector3f& wp) const {
	Eigen::Vector4f p = view_projection_transformation() * wp.homogeneous();
	p /= p[3];
	for(std::ptrdiff_t i = 0; i < 3; ++i) if(p[i] < -1 || p[i] > 1) return false;
	return true;
}


bool projection_camera::has_viewing_frustum() const {
	return true;
}


projection_frustum projection_camera::relative_viewing_frustum() const {
	return frustum_;
}


void projection_camera::set_relative_viewing_frustum(const projection_frustum& fr) {
	frustum_ = fr;
}



Eigen::Projective3f projection_camera::projection_transformation() const {
	return Eigen::Projective3f(frustum_.matrix);
}


float projection_camera::projected_depth(const Eigen::Vector3f& wp) const {
	Eigen::Vector4f projected = view_projection_transformation() * wp.homogeneous();
	return projected[2] / projected[3];
}


Eigen::Vector2f projection_camera::to_projected(const Eigen::Vector3f& p) const {
	Eigen::Vector4f projected = view_projection_transformation() * p.homogeneous();
	return Eigen::Vector2f( projected[0]/projected[3], projected[1]/projected[3] );
}

Eigen::Vector2f projection_camera::to_projected(const Eigen::Vector3f& p, float& depth) const {
	Eigen::Vector4f projected = view_projection_transformation() * p.homogeneous();
	projected /= projected[3];
	depth = projected[2];
	return Eigen::Vector2f( projected[0], projected[1] );
}


Eigen::Vector3f projection_camera::point(const Eigen::Vector2f& projected, float z) const {
	Eigen::Vector4f p(projected[0], projected[1], z, 1);
	p = view_projection_transformation().inverse() * p;
	return (p / p[3]).head(3);
}



}
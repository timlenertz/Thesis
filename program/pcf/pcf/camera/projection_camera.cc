#include "projection_camera.h"

namespace pcf {

projection_camera::projection_camera(const pose& ps, const projection_frustum& fr) :
	camera(ps), frustum_(fr) { }


projection_camera::projection_camera(const camera& cam) :
	camera(cam.pose_), frustum_(cam.relative_viewing_frustum()) { }


angle projection_camera::angle_on_near_plane_between_(const Eigen::Vector2f& a, const Eigen::Vector2f& b) const {
	Eigen::Vector4f pa(a[0], a[1], -1, 1), pb(b[0], b[1], -1, 1);
	Eigen::Projective3f inv_mat = projection_transformation().inverse();
	pa = inv_mat * pa; pa /= pa[3];
	pb = inv_mat * pb; pb /= pb[3];
	return std::acos( pa.head(3).dot(pb.head(3)) );
}


angle projection_camera::field_of_view_x() const {
	return angle_on_near_plane_between_(Eigen::Vector2f(-1, 0), Eigen::Vector2f(+1, 0));
}


angle projection_camera::field_of_view_y() const {
	return angle_on_near_plane_between_(Eigen::Vector2f(0, -1), Eigen::Vector2f(0, +1));
}


std::array<angle, 2> projection_camera::field_of_view_limits_x() const {
	return {
		angle_on_near_plane_between_(Eigen::Vector2f(0, 0), Eigen::Vector2f(-1, 0)),
		angle_on_near_plane_between_(Eigen::Vector2f(+1, 0), Eigen::Vector2f(0, 0))
	};
}

std::array<angle, 2> projection_camera::field_of_view_limits_y() const {
	return {
		angle_on_near_plane_between_(Eigen::Vector2f(0, 0), Eigen::Vector2f(0, -1)),
		angle_on_near_plane_between_(Eigen::Vector2f(0, +1), Eigen::Vector2f(0, 0))
	};
}


bool projection_camera::in_field_of_view(const Eigen::Vector3f& p) const {
	Eigen::Vector4f p = view_projection_transformation() * p.homogeneous();
	p /= p[3];
	for(std::ptrdiff_i = 0; i < 3; ++i) if(p[i] < -1 || p[i] > 1) return false;
	return true;
}


bool projection_camera::has_viewing_frustum() const {
	return true;
}


projection_frustum projection_camera::relative_viewing_frustum() const {
	return frustum_;
}


Eigen::Projective3f projection_camera::projection_transformation() const {
	return Eigen::Projective3f(frustum_.matrix);
}


float projection_camera::projected_depth(const Eigen::Vector3f&) const {
	Eigen::Vector4f projected = view_projection_transformation() * p.homogeneous();
	return projected[2] / projected[3];
}


Eigen::Vector2f projection_camera::to_projected(const Eigen::Vector3f& p) const {
	Eigen::Vector4f projected = view_projection_transformation() * p.homogeneous();
	return Eigen::Vector2f( projected[0]/projected[3], projected[1]/projected[3] );
}

Eigen::Vector3f projection_camera::point(const Eigen::Vector2f& projected, float z) const {
	Eigen::Vector4f p(projected[0], projected[1], z, 1);
	p = view_projection_transformation().inverse() * p;
	return (p / p[3]).head(3);
}



}
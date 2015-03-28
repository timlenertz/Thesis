#include "point_full.h"

namespace pcf {

point_full& point_full::operator=(const point_xyz& pt) {
	point_xyz::operator=(pt);
	color = default_color();
	normal_a_ = 0.0;
	normal_b_ = 0.0;
	weight_ = NAN;
	return *this;
}

Eigen::Vector3f point_full::get_normal(bool normalized) const {
	return Eigen::Vector3f(
		(normal_b_ + normal_a_) / 2.0f,
		(normal_b_ - normal_a_) / 2.0f,
		std::sqrt(1.0f - normal_a_*normal_b_)
	);
}

void point_full::set_normal(const Eigen::Vector3f& n) {
	Eigen::Vector3f nn = n.normalized();
	normal_a_ = nn[0] - nn[1];
	normal_b_ = nn[0] + nn[1];
}

void point_full::swap(point_full& p) {
	homogeneous_coordinates.swap(p.homogeneous_coordinates);
	std::swap(color, p.color);
	std::swap(normal_a_, p.normal_a_);
	std::swap(normal_b_, p.normal_b_);
	std::swap(weight_, p.weight_);
}

std::ostream& operator<<(std::ostream& str, const point_full& p) {
	if(p.valid()) {
		str << '(' << p[0] << ", " << p[1] << ", "<< p[2] << ')';
		// TODO
	} else {
		str << "(none)";
	}
	return str;
}

}
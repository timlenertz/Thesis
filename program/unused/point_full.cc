#include "point_full.h"
#include "point_ext.h"

namespace pcf {

point_full::point_full(const point_ext& pt) :
	point_xyz(pt),
	color(pc.color)
{
	set_normal(pt.get_normal());
	set_weight(pt.get_weight());
}
	

point_full& point_full::operator=(const point_xyz& pt) {
	point_xyz::operator=(pt);
	color = 0xffffff;
	normal_a_ = 0.0;
	normal_b_ = 0.0;
	has_normal_ = false;
	weight_ = NAN;
	return *this;
}

point_full& point_full::operator=(const point_ext& pt) {
	point_xyz::operator=(pt);
	color = pt.get_color();
	set_normal(pt.get_normal());
	weight_ = pt.get_weight();
	return *this;
}

Eigen::Vector3f point_full::get_normal(bool normalized) const {
	float z = std::sqrt(1.0f - normal_a_*normal_b_);
	return Eigen::Vector3f(
		(normal_b_ + normal_a_) / 2.0f,
		(normal_b_ - normal_a_) / 2.0f,
		normal_z_sign_ ? z : -1
	);
}

void point_full::set_normal(const Eigen::Vector3f& n) {
	Eigen::Vector3f nn = n.normalized();
	normal_a_ = nn[0] - nn[1];
	normal_b_ = nn[0] + nn[1];
	normal_z_sign_ = (nn[2] > 0.0);
}

void point_full::flip_normal() {
	normal_a_ = -normal_a_;
	normal_b_ = -normal_b_;
	normal_z_sign_ = ! normal_z_sign_;
}

void point_full::swap(point_full& p) {
	homogeneous_coordinates.swap(p.homogeneous_coordinates);
	std::swap(color, p.color);
	std::swap(normal_a_, p.normal_a_);
	std::swap(normal_b_, p.normal_b_);
	std::swap(weight_, p.weight_);
	std::swap(flags_, p.flags_);
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
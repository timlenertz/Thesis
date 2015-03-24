#include "point_full.h"

namespace pcf {

point_full& point_full::operator=(const point_xyz& pt) {
	point_xyz::operator=(pt);
	normal = no_normal_();
	color = default_color();
	has_normal_ = false;
	zero_weight_ = false;
	return *this;
}

Eigen::Vector3f point_full::get_normal(bool normalized) const {
	if(has_normal_) {
		if(normalized || zero_weight_) return normal.normalized();
		else return normal;
	} else {
		return Eigen::Vector3f::Zero();
	}
}

void point_full::set_normal(const Eigen::Vector3f& n) {
	if(n.isZero()) {
		normal = no_normal_(get_weight());
		has_normal_ = false;
	} else {
		normal = get_weight() * n.normalized();
		has_normal_ = true;	
	}
}

float point_full::get_weight() const {
	if(zero_weight_) return 0.0;
	else return normal.squaredNorm();
}

void point_full::set_weight(float w) {
	normal.normalize();
	if(w == 0.0) {
		zero_weight_ = true;
	} else {
		normal *= std::sqrt(w);
		zero_weight_ = false;
	}
}

void point_full::swap(point_full& p) {
	homogeneous_coordinates.swap(p.homogeneous_coordinates);
	normal.swap(p.normal);
	std::swap(color, p.color);
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
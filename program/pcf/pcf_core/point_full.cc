#include "point_full.h"
#include <iostream>

namespace pcf {

point_full::point_full() :
	point_xyz(), normal(Eigen::Vector3f::Zero()), color(default_color()) { }


point_full::point_full(const point_xyz& pt) :
	point_xyz(pt), normal(Eigen::Vector3f::Zero()), color(default_color()) { }


point_full::point_full(float x, float y, float z, std::uint8_t r, std::uint8_t g, std::uint8_t b) :
	point_xyz(x, y, z), color(r, g, b) { }


point_full& point_full::operator=(const point_xyz& pt) {
	point_xyz::operator=(pt);
	normal = Eigen::Vector3f::Zero();
	color = default_color();
	return *this;
}


}
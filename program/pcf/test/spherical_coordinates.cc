#include <catch.hpp>
#include "../pcf/geometry/spherical_coordinates.h"
#include "helper.h"
#include <utility>

using namespace pcf;

TEST_CASE("Spherical coordinates", "[geometry]") {
	SECTION("Cartesian to Spherical and back") {
		Eigen::Vector3f cs[] = {
			Eigen::Vector3f(1.0, 0.0, 0.0),
			Eigen::Vector3f(0.0, 1.0, 0.0),
			Eigen::Vector3f(0.0, 0.0, 1.0),
			Eigen::Vector3f(12.0, 23.0, 14.0),
			Eigen::Vector3f(12.0, -23.0, 14.0),
			Eigen::Vector3f(-12.0, 23.0, 14.0),
			Eigen::Vector3f(-12.0, -23.0, 14.0),
			Eigen::Vector3f(12.0, 23.0, -14.0),
			Eigen::Vector3f(12.0, -23.0, -14.0),
			Eigen::Vector3f(-12.0, 23.0, -14.0),
			Eigen::Vector3f(-12.0, -23.0, -14.0)
		};
		
		for(const Eigen::Vector3f& c : cs) {
			spherical_coordinates s = spherical_coordinates::from_cartesian(c);
			Eigen::Vector3f c2 = s.to_cartesian();
			REQUIRE( (c2 - c).norm() < 0.01 ); // account for fp imprecision
		}
	}
}

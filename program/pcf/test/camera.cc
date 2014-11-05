#include <catch.hpp>
#include "../pcf/geometry/camera.h"
#include "helper.h"
#include <utility>
#include <cmath>

using namespace pcf;

TEST_CASE("Camera") {
	camera cam(pose(), 60.0, 4.0/3.0);
	
	SECTION("Spherical <-> Cartesian") {
		Eigen::Vector3f c(0.0, 1.0, 1.0);
		auto s = cam.to_spherical(c);
		auto c2 = cam.from_spherical(s);
		WARN(s[0] << "  " << s[1] << "  " << s[2]);
		WARN(c2);
	}
}

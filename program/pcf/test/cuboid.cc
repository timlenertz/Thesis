#include "cuboid.h"

#include <cppunit/extensions/HelperMacros.h>
#include <random>

CPPUNIT_TEST_SUITE_REGISTRATION(pcf::cuboid_test);

namespace pcf {

Eigen::Vector3f cuboid_test::random_point_in_cuboid_(const cuboid& cub) {
	std::default_random_engine gen;
	std::uniform_real_distribution<float> dist_x(cub.origin[0], cub.extremity[0]);
	std::uniform_real_distribution<float> dist_y(cub.origin[1], cub.extremity[1]);
	std::uniform_real_distribution<float> dist_z(cub.origin[2], cub.extremity[2]);
	return Eigen::Vector3f( dist_x(gen), dist_y(gen), dist_z(gen) );
}


cuboid cuboid_test::random_cuboid_() {
	std::default_random_engine gen;
	std::uniform_real_distribution<float> dist(0.0, 100.0);
	float o[3] = { dist(gen), dist(gen), dist(gen) };
	return cuboid(
		Eigen::Vector3f(o[0], o[1], o[2]),
		Eigen::Vector3f(o[0] + dist(gen), o[1] + dist(gen), o[2] + dist(gen))
	);
}


void cuboid_test::setUp() {

}

void cuboid_test::tearDown() {

}

void cuboid_test::test_minimal_maximal_cuboid_distance() {
	int nb_max_cuboids = 10;
	int nb_points = 100;

	for(int i = 0; i < nb_max_cuboids; ++i) {
		cuboid a = random_cuboid_(), b = random_cuboid_();
		if(a.contains(b)) continue;
		
		float mn = cuboid::minimal_distance_sq(a, b);
		float mx = cuboid::maximal_distance_sq(a, b);
		
		for(int j = 0; j < nb_points; ++j) {
			auto pa = random_point_in_cuboid_(a);
			auto pb = random_point_in_cuboid_(b);
			auto d = (pa - pb).squaredNorm();
			
			CPPUNIT_ASSERT(d >= mn && d <= mx);
		}
	}
}

}
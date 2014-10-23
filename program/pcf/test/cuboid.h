#ifndef PCFTEST_CUBOID_H_
#define PCFTEST_CUBOID_H_

#include <cppunit/extensions/HelperMacros.h>
#include <Eigen/Eigen>

#include "../src/geometry/cuboid.h"

namespace pcf {

class cuboid_test : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(cuboid_test);
	CPPUNIT_TEST(test_minimal_maximal_cuboid_distance);
	CPPUNIT_TEST_SUITE_END();

private:
	cuboid* cub_a_;
	cuboid* cub_b_;

	static Eigen::Vector3f random_point_in_cuboid_(const cuboid&);
	static cuboid random_cuboid_();

public:
	void setUp();
	void tearDown();
	
	void test_minimal_maximal_cuboid_distance();
};

}

#endif
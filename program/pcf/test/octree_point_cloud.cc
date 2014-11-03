#include <catch.hpp>

#include "../pcf/point_cloud/tree_point_cloud.h"
#include "../pcf/point_cloud/tree_traits/octree_traits.h"
#include "../pcf/point.h"
#include "../pcf/util/random.h"
#include "helper.h"
#include <utility>
#include <cmath>

using namespace pcf;

TEST_CASE("Octree Point cloud") {
	using cloud = tree_point_cloud<octree_traits, point_xyz>;
	
	cloud pc( example_model("bunny"), 10 );
	
	SECTION("Leaf containing point") {
		point_xyz p = pc[ random_integer(pc.size()) ];
		auto l = pc.root().deepest_child_containing_point(p);
		REQUIRE(l.is_leaf());
		REQUIRE(l.box().contains(p));
	}
}

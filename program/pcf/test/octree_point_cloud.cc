#include "catch.hpp"

#include "../src/point_cloud/tree_point_cloud.h"
#include "../src/point_cloud/tree_traits/octree_traits.h"
#include "../src/point.h"
#include "../src/util/random.h"
#include "helper.h"
#include <utility>
#include <cmath>

using namespace pcf;

TEST_CASE("Octree Point cloud") {
	using cloud = tree_point_cloud<octree_traits, point_xyz>;
	
	cloud pc( bunny_model(), 10 );
	
	SECTION("Leaf containing point") {
		point_xyz p = pc[ random_integer(pc.size()) ];
		auto l = pc.root().deepest_child_containing_point(p);
		REQUIRE(l.is_leaf());
		REQUIRE(l.cub().contains(p));
	}
	
	SECTION("Neighboring nodes MinDist order") {
		point_xyz p = pc[ random_integer(pc.size()) ];
		cloud::backtrace bt; 
		auto l = pc.root().deepest_child_containing_point(p, bt);

		float last_d = 0;
		l.visit_neightboring_leaves([&last_d, &l](auto&& n) {
			float d = cuboid::minimal_distance_sq(n.cub(), l.cub());
			REQUIRE(d >= last_d);
			last_d = d;
			return true;
		}, [&l](auto&& a, auto&& b) {
			float da = cuboid::minimal_distance_sq(a.cub(), l.cub());
			float db = cuboid::minimal_distance_sq(b.cub(), l.cub());
			return (da < db);
		}, bt);
	}
	
	SECTION("Neighboring nodes MaxDist order") {
		point_xyz p = pc[ random_integer(pc.size()) ];
		cloud::backtrace bt; 
		auto l = pc.root().deepest_child_containing_point(p, bt);

		float last_d = 0;
		l.visit_neightboring_leaves([&last_d, &l](auto&& n) {
			float d = cuboid::maximal_distance_sq(n.cub(), l.cub());
			REQUIRE(d >= last_d);
			last_d = d;
			return true;
		}, [&l](auto&& a, auto&& b) {
			float da = cuboid::maximal_distance_sq(a.cub(), l.cub());
			float db = cuboid::maximal_distance_sq(b.cub(), l.cub());
			return (da < db);
		}, bt);
	}
}

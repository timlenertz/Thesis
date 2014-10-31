#include <catch.hpp>
#include <iostream>

#include "../src/point_cloud/grid_point_cloud.h"
#include "../src/point.h"
#include "../src/util/random.h"
#include "helper.h"
#include <utility>
#include <cmath>

using namespace pcf;

TEST_CASE("Grid Point cloud") {
	using cloud = grid_point_cloud<point_xyz>;
	
	//float c = cloud::optimal_cell_length_for_knn(bunny_model(), 10, 0.2);
	cloud pc( bunny_model(), 0.01 );
	
	auto random_nonempty_cell = [&pc]() -> cloud::cell_coordinates {
		for(;;) {
			cloud::cell_coordinates c;
			for(std::ptrdiff_t i = 0; i < 3; ++i) c[i] = random_integer(pc.number_of_cells(i));
			bounding_box b = pc.cell_subspace(c).box();
			for(auto&& p : pc) if(b.contains(p)) return c;
		}
	};
	
	SECTION("Size") {
	}
	
	SECTION("Subspace Points") {
		auto test = [&pc](cloud::subspace s) {
			bounding_box b = s.box();
			
			std::size_t real_n = 0;
			for(auto&& p : pc) if(b.contains(p)) ++real_n;
			
			std::size_t n = 0;
			for(auto&& p : s) {
				REQUIRE(b.contains(p));
				++n;
			}
			REQUIRE(n == real_n);
		};
	
		for(std::ptrdiff_t i = 0; i < 10; ++i) {
			auto c = random_nonempty_cell();
			auto s = pc.cell_subspace(c);
			test(s);
			s.expand(2);
			test(s);
		}
		test(pc.full_subspace());
	}
}

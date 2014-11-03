#include <catch.hpp>
#include <iostream>

#include "../pcf/point_cloud/grid_point_cloud.h"
#include "../pcf/point.h"
#include "../pcf/util/random.h"
#include "helper.h"
#include <utility>
#include <cmath>

using namespace pcf;

TEST_CASE("Grid Point cloud") {
	using cloud = grid_point_cloud<point_xyz>;

	auto random_nonempty_cell = [](cloud& pc) -> cloud::cell_coordinates {
		for(;;) {
			cloud::cell_coordinates c;
			for(std::ptrdiff_t i = 0; i < 3; ++i) c[i] = random_integer(pc.number_of_cells(i));
			bounding_box b = pc.cell_subspace(c).box();
			for(auto&& p : pc) if(b.contains(p)) return c;
		}
	};
	
	SECTION("Size") {
	}
	
	
	SECTION("k Nearest Neighbors") {
		auto pc1 = example_model("comet");
		float c = optimal_grid_cell_length_for_knn(pc1, 10, 0.2);
		cloud pc(std::move(pc1), c);

		SECTION("Finds >= 10 points each time") {
			pc.find_nearest_neighbors(10, [](auto&& p) {
				return true;
			}, [](auto&& p, auto&& knn) {
				REQUIRE(knn.size() >= 10);
			});		
		}
		
		
	}
	
	
	SECTION("Subspace Points") {
		cloud pc(example_model("bunny"), 0.05);

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
			auto c = random_nonempty_cell(pc);
			auto s = pc.cell_subspace(c);
			test(s);
			s.expand(2);
			test(s);
		}
		test(pc.full_subspace());
	}
}

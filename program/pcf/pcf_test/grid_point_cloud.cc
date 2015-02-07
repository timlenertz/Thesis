#include <catch.hpp>
#include <iostream>

#include "../pcf_core/point_cloud/grid/grid_point_cloud.h"
#include "../pcf_core/point.h"
#include "../pcf_core/util/random.h"
#include "helper.h"
#include <utility>
#include <cmath>

using namespace pcf;

TEST_CASE("Grid Point cloud") {
	using cloud = grid_point_cloud<point_xyz>;
	using cell_coordinates = cloud::cell_coordinates;

	auto random_nonempty_cell = [](cloud& pc) -> cloud::cell_coordinates {
		for(;;) {
			cloud::cell_coordinates c;
			for(std::ptrdiff_t i = 0; i < 3; ++i) c[i] = random_integer(pc.number_of_cells(i));
			
			bounding_box b = pc.cell_subspace(c).box();
			for(auto&& p : pc) if(b.contains(p)) return c;
		}
	};
	
	SECTION("Size") {
		cloud pc(example_model("bunny"), 0.01);
		REQUIRE(pc.number_of_cells() == pc.number_of_cells(0)*pc.number_of_cells(1)*pc.number_of_cells(2));
	}
	
	
	SECTION("k Nearest Neighbors") {
		/*auto pc1 = example_model("comet");
		float c = optimal_grid_cell_length_for_knn(pc1, 10, 0.2);
		cloud pc(std::move(pc1), c);

		SECTION("Finds >= 10 points each time") {
			pc.find_nearest_neighbors(10, [](const point_xyz& p) {
				return true;
			}, [](const point_xyz& p, std::vector<point_xyz*>& knn) {
				REQUIRE(knn.size() >= 10);
			});		
		}*/
	}
	
	
	SECTION("Subspace Points") {
		cloud pc(example_model("bunny"), 0.015);
		const cell_coordinates cc[] {
			cell_coordinates(0, 0, 0), // origin
			cell_coordinates(17, 16, 14), // extremity
			
			// on corner/edge
			cell_coordinates(0, 16, 14),
			cell_coordinates(17, 0, 14),
			cell_coordinates(17, 16, 0),
			cell_coordinates(0, 16, 0),
			
			// non-empty
			cell_coordinates(1, 1, 4),
			cell_coordinates(1, 5, 7),
			cell_coordinates(6, 2, 1),
			cell_coordinates(9, 0, 5),
			cell_coordinates(2, 5, 3)
		};
		
		auto test = [&pc](const cloud::subspace& s) {				
			bounding_box b = s.box();
			
			// Count number of points in subspace bounding box -> take as real value
			std::size_t real_n = 0;
			for(auto&& p : pc) {
				if(b.contains(p)) ++real_n;
				// If point belongs in cell of subspace, it must belong into bounding box
				REQUIRE(s.contains(pc.cell_for_point(p)) == b.contains(p));
			}
			
			// Verify subspace point counting algorithm
			REQUIRE(s.number_of_points() == real_n);
			
			// Create segment union from subspace, and verify segment union point cound
			auto segs = pc.segment_union_for_subspace(s);
			REQUIRE(segs.size() == real_n);

			std::size_t n = 0;
			for(auto&& p : segs) {
				REQUIRE(b.contains(p));
				++n;
			}
			REQUIRE(n == real_n);
		};

	
		for(const cell_coordinates& c : cc) {
			auto s = pc.cell_subspace(c);
			test(s);
			s.expand(2);
			test(s);
		}
	}
}

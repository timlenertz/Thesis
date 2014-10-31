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
	
	SECTION("Size") {
	}
	
	SECTION("Subspace") {
		std::size_t n = 0;
		std::ptrdiff_t i[3];
		while(n == 0) {
			i[0] = random_integer(pc.number_of_cells(0));
			i[1] = random_integer(pc.number_of_cells(1));
			i[2] = random_integer(pc.number_of_cells(2));
		
			auto s = pc.cell_subspace({i[0], i[1], i[2]});
		
			n = 0;
			for(auto&& p : pc) if(s.box().contains(p)) ++n;
		}	
				
		
		auto s = pc.cell_subspace({i[0], i[1], i[2]});
		s.expand();
		
		std::cout << "0 = (" << i[0] << ", " << i[1] << ", " << i[2] << ")" << std::endl;
		std::cout << "(" << s.origin[0] << ", " << s.origin[1] << ", " << s.origin[2] << ") --> ("
			<< s.extremity[0] << ", " << s.extremity[1] << ", " << s.extremity[2] << ")" << std::endl;
		
		
		
		n = 0;
		for(auto&& p : pc) if(s.box().contains(p)) ++n;
		std::cout << "real in bb: " << n << std::endl;
		
		
		
		n = 0;
		for(auto it = s.begin(); it != s.end(); ++it) {
			if(! s.box().contains(*it)) std::cout << '.' << std::flush;
			++n;
		}
		std::cout << "iterated: " << n << std::endl;
	}
}

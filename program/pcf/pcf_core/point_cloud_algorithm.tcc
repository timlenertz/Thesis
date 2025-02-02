#include <vector>
#include <algorithm>
#include "point.h"
#include "point_algorithm.h"
#include "point_filter/accept.h"
#include "point_filter/probability.h"
#include "point_cloud/merge.h"
#include "util/random.h"
#include "geometry/math_constants.h"
#include "field/field.h"
#include <iostream>

namespace pcf {

template<typename Cloud>
float median_closest_neighbor_distance(const Cloud& pc, std::size_t samples) {
	std::vector<float> distances(samples);
	
	#pragma omp parallel for
	for(std::ptrdiff_t i = 0; i < samples; ++i) {
		const auto& p1 = pc.random_point();
		const auto& p2 = pc.closest_point(
			p1, 
			0,
			INFINITY,
			[&p1](const typename Cloud::point_type& p) { return (&p != &p1); }
		);
		distances[i] = distance(p1, p2);
	}
	
	auto median_it = distances.begin() + distances.size()/2;
	std::nth_element(distances.begin(), distances.end(), median_it);
	return *median_it;
}




template<typename Cloud>
void test_knn(Cloud& pc) {
	auto got_knn = [&](point_full& pt, typename Cloud::selection_iterator& knn_begin, typename Cloud::selection_iterator& knn_end) {
		set_unique_color(knn_begin, knn_end, rgb_color::red);
	}; 
	pc.nearest_neighbors(100, probability_point_filter(10.0 / pc.number_of_valid_points()), got_knn);
}




template<typename Point>
std::array<unorganized_point_cloud<Point>, 2> split_random_downsampled(const point_cloud<Point>& pc, float ratio) {
	unorganized_point_cloud<Point> p = pc, q = pc;
	p.downsample_random(ratio);
	auto mask = p.valid_points_mask();
	q.filter_mask(mask, false);
	return {p, q};
}


}
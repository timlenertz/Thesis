#include <vector>
#include <algorithm>
#include "point.h"
#include "point_algorithm.h"
#include "point_filter/accept.h"
#include "point_filter/probability.h"
#include "util/random.h"
#include "geometry/math_constants.h"
#include <iostream>

namespace pcf {

template<typename Cloud>
float median_closest_point_distance(const Cloud& pc, std::size_t samples) {
	std::vector<float> distances(samples);
	
	#pragma omp parallel for
	for(std::ptrdiff_t i = 0; i < samples; ++i) {
		const auto& p1 = pc.random_point();
		const auto& p2 = pc.closest_point(p1);
		distances[i] = distance(p1, p2);
	}
	
	auto median_it = distances.begin() + distances.size()/2;
	std::nth_element(distances.begin(), distances.end(), median_it);
	return *median_it;
}


template<typename Cloud>
void compute_normals(Cloud& pc) {
	auto got_knn = [&](point_full& pt, typename Cloud::selection_iterator& knn_begin, typename Cloud::selection_iterator& knn_end) {
		plane pla = fit_plane_to_points(knn_begin, knn_end);
		
		pt.set_normal( pla.normal );
	}; 
	pc.nearest_neighbors(10, probability_point_filter(1.0), got_knn, false);
}


template<typename Cloud>
void set_local_density_weights(Cloud& pc, std::size_t k) {
	auto got_knn = [&](point_full& p, typename Cloud::selection_iterator& knn_begin, typename Cloud::selection_iterator& knn_end) {	
		auto sq_dist = [&p](const point_full& q) { return distance_sq(p, q); };
		auto cmp = [&sq_dist](const point_full& a, const point_full& b) { return sq_dist(a) < sq_dist(b); };
		auto max_distance_it = std::max_element(knn_begin, knn_end, cmp);
		
		std::ptrdiff_t n = knn_end - knn_begin;
		float area = pi * sq_dist(*max_distance_it);
		std::cout << n << " ar:" << area << std::endl;
		float density = (float)n / area;
		
		p.set_weight(density);
	}; 
	pc.nearest_neighbors(k, accept_point_filter(), got_knn, false);
}


}
#include <vector>
#include <algorithm>
#include "point.h"
#include "point_algorithm.h"
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
	auto got_knn = [&](point_full& pt, const typename Cloud::selection& knn) {
		plane pla = fit_plane_to_points(knn.begin(), knn.end());
		
		pt.normal = pla.normal;
	}; 
	pc.nearest_neighbors(10, probability_point_filter(1.0), got_knn, false);
}


template<typename Cloud>
void set_local_density_weights(Cloud& pc) {
	std::size_t k = 20;
	auto got_knn = [&](point_full& p, const typename Cloud::selection& knn) {
		auto sq_dist = [&p](const point_full& q) { return distance_sq(p, q); };
		auto cmp = [&sq_dist](const point_full& a, const point_full& b) { return sq_dist(a) < sq_dist(b); };
		auto max_distance_it = std::max_element(knn.begin(), knn.end(), cmp);
		float max_distance = distance(p, *max_distance_it);
		
		float area = pi * max_distance*max_distance;
		float density = (float)k / area;
		
		p.color = (density - 400000.0)/500000.0 * rgb_color::white;
	}; 
	pc.nearest_neighbors(k, probability_point_filter(1.0), got_knn, false);
}


}
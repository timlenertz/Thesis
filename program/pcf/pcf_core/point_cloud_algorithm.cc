#include "point_cloud_algorithm.h"
#include "geometry/math_constants.h"
#include <vector>
#include <cmath>

namespace pcf {

unorganized_point_cloud_full make_sample_with_displaced_points(const point_cloud_full& pc_orig, std::size_t num_copies, const pose& cam_ps, float p_l) {
	random_generator& rng = get_random_generator();
	std::vector<unorganized_point_cloud_full> copies(num_copies, pc_orig);
	for(auto& pc : copies) pc.erase_invalid_points();
	for(auto& pc : copies) for(auto& p : pc) {
		const Eigen::Vector3f& pn = p.get_normal();
		if(pn.isZero()) { p.invalidate(); continue; }
		
		Eigen::Vector3f n = cam_ps.transformation_from_world().linear() * pn;
		float nx = n[0], ny = n[1], nz = n[2];
		
		float lmin = p_l * std::min({
			std::sqrt(1.0 + sq(nx)/sq(nz)),
			std::sqrt(1.0 + sq(ny)/sq(nz)),
			std::sqrt(2.0 + sq(nx+ny)/sq(nz)),
			std::sqrt(2.0 + sq(nx-ny)/sq(nz))
		});

		float k = 2.5;
		
		float r = std::uniform_real_distribution<float>(0.0, (k*lmin)/2.0)(rng);
		float theta = std::uniform_real_distribution<float>(0.0, two_pi)(rng);
		
		Eigen::Vector3f d(r * std::cos(theta), r * std::sin(theta), 0.0);
		d = Eigen::Quaternionf::FromTwoVectors(pn, Eigen::Vector3f::UnitZ()) * d;
		
		p.homogeneous_coordinates.head(3) += d;
	}
	return merge_point_clouds(copies.begin(), copies.end());
}


void compute_normals(grid_point_cloud_full& pc) {
	auto got_knn = [&](point_full& pt, typename grid_point_cloud_full::selection_iterator& knn_begin, typename grid_point_cloud_full::selection_iterator& knn_end) {
		plane pla = compute_tangent_plane(pt, knn_begin, knn_end);
		pt.set_normal( pla.normal );
	}; 
	pc.nearest_neighbors(10, accept_point_filter(), got_knn);
}



void compute_local_density_weights(grid_point_cloud_full& pc, std::size_t k) {
	auto got_knn = [&](point_full& p, typename grid_point_cloud_full::selection_iterator& knn_begin, typename grid_point_cloud_full::selection_iterator& knn_end) {	
		float density = compute_local_surface_density(p, knn_begin, knn_end);
		p.set_weight(density);
	}; 
	pc.nearest_neighbors(k, accept_point_filter(), got_knn);
}


void compute_local_curvature_weights(grid_point_cloud_full& pc, std::size_t k, float r, float A, float D) {
	std::ptrdiff_t i = 0;

	float r_sq = r * r;
	auto got_knn = [&](point_full& p, typename grid_point_cloud_full::selection_iterator& knn_begin, typename grid_point_cloud_full::selection_iterator& knn_end) {
		float density = compute_local_surface_density(p, knn_begin, knn_end);
		float k = std::ceil(density * pi * r_sq);
		if(k < 10) k = 10;
		if(k > std::distance(knn_begin, knn_end)) k = std::distance(knn_begin, knn_end);
		float curvature = compute_local_surface_curvature(p, knn_begin, knn_begin + k, A, D);
		p.set_weight(curvature);
		
		++i;
		if(i % 5000 == 0) std::cout << i << " of " << pc.size() << std::endl;
	}; 
	pc.nearest_neighbors(k, accept_point_filter(), got_knn);	
}


}
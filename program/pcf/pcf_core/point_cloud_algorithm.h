#ifndef PCF_POINT_CLOUD_ALGORITHM_H_
#define PCF_POINT_CLOUD_ALGORITHM_H_

#include "point_cloud/point_cloud.h"
#include "point_cloud/unorganized/unorganized_point_cloud.h"
#include "point_cloud/grid/grid_point_cloud.h"
#include <array>
#include <cmath>

namespace pcf {

/**
Estimate median closest point distance.
Takes given number of random samples, and uses point cloud's algorithm to find closest point.
*/
template<typename Cloud>
float median_closest_neighbor_distance(const Cloud&, std::size_t samples = 10000);

void compute_normals(grid_point_cloud_full&);

void compute_local_density_weights(grid_point_cloud_full&, std::size_t k);

void compute_local_curvature_weights(grid_point_cloud_full&, std::size_t k, float r, float a, float d);

template<typename Cloud>
void test_knn(Cloud&);

template<typename Point>
std::array<unorganized_point_cloud<Point>, 2> split_random_downsampled(const point_cloud<Point>&, float ratio);


unorganized_point_cloud_full make_sample_with_displaced_points(const point_cloud_full& pc_orig, std::size_t num_copies, const pose& cam_ps, float p_l);



}

#include "point_cloud_algorithm.tcc"

#endif
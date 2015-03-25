#ifndef PCF_POINT_CLOUD_ALGORITHM_H_
#define PCF_POINT_CLOUD_ALGORITHM_H_

#include "point_cloud/point_cloud.h"
#include "point_cloud/grid/grid_point_cloud.h"

namespace pcf {

/**
Estimate median closest point distance.
Takes given number of random samples, and uses point cloud's algorithm to find closest point.
*/
template<typename Cloud>
float median_closest_point_distance(const Cloud&, std::size_t samples = 100);

template<typename Cloud>
void compute_normals(Cloud&);

template<typename Cloud>
void compute_local_density_weights(Cloud&, std::size_t k, float ratio = 1.0);


extern template void compute_local_density_weights(grid_point_cloud_full&, std::size_t, float);

}

#include "point_cloud_algorithm.tcc"

#endif
#ifndef PCF_POINT_CLOUD_ALGORITHM_H_
#define PCF_POINT_CLOUD_ALGORITHM_H_

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
void set_local_density_weights(Cloud&);

}

#include "point_cloud_algorithm.tcc"

#endif
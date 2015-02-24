#ifndef PCF_POINT_CLOUD_ALGORITHM_H_
#define PCF_POINT_CLOUD_ALGORITHM_H_

namespace pcf {

/**
Estimate median closest point distance.
Takes given number of random samples, and uses point cloud's algorithm to find closest point.
*/
template<typename Cloud>
float estimate_median_closest_point_distance(const Cloud&, std::size_t samples = 100);

}

#include "point_cloud_algorithm.tcc"

#endif
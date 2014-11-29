#ifndef PCF_POINT_CLOUD_ALGORITHM_H_
#define PCF_POINT_CLOUD_ALGORITHM_H_

namespace pcf {

template<typename Cloud>
float estimate_median_closest_point_distance(const Cloud&, std::size_t samples = 100);

}

#include "point_cloud_algorithm.tcc"

#endif
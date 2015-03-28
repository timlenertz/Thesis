#include "point_cloud_algorithm.h"

namespace pcf {

template void compute_local_density_weights(grid_point_cloud_full&, std::size_t, float);
template void compute_normals(grid_point_cloud_full&);

}
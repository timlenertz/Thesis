#ifndef PCF_ARTIFICIAL_POINT_CLOUD_H_
#define PCF_ARTIFICIAL_POINT_CLOUD_H_

#include "unorganized_point_cloud.h"
#include "../../geometry/bounding_box.h"

namespace pcf {

/// Create point cloud of a sphere centered at origin.
/// The points are randomly distributed on the surface at a uniform density.
unorganized_point_cloud_full make_sphere_point_cloud(float radius, std::size_t number_of_points);

unorganized_point_cloud_full make_sphere_point_cloud_with_density(float radius, float density);

unorganized_point_cloud_full make_relief_point_cloud(float width, float density);

}

#endif
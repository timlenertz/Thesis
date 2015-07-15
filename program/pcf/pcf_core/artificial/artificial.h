#ifndef PCF_ARTIFICIAL_POINT_CLOUD_H_
#define PCF_ARTIFICIAL_POINT_CLOUD_H_

#include "../point_cloud/unorganized/unorganized_point_cloud.h"
#include "../point_cloud/range/range_point_cloud.h"
#include "../point_cloud/range/camera_range_point_cloud.h"
#include "../geometry/bounding_box.h"
#include <vector>
#include <Eigen/Eigen>

namespace pcf {

unorganized_point_cloud_full make_sphere_point_cloud(float radius, float density);
std::vector<point_full> make_sphere_samples(std::size_t n, float radius);

range_point_cloud_full make_plane_point_cloud(float side_length, float density);
unorganized_point_cloud_full make_plane_point_cloud_random_distribution(float side_length, float density);
std::vector<point_full> make_plane_samples(std::size_t n, float side_length);

}

#endif
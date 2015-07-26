#ifndef PCF_POINT_ALGORITHM_H_
#define PCF_POINT_ALGORITHM_H_

#include <Eigen/Eigen>
#include <cstdint>
#include "point.h"
#include "geometry/plane.h"
#include "geometry/bounding_box.h"
#include "geometry/angle.h"
#include "point_cloud/selection.h"

namespace pcf {

/**
Iterate through points and find closest point to reference point.
If any point is closer than \a accepting_distance, that one may be accepted. Point cloud classes implement more efficient algorithms for this based on their structure.
*/
template<typename Point, typename Iterator>
Iterator find_closest_point(const Point& ref, Iterator begin, Iterator end, float accepting_distance = 0);

template<typename Point, typename Iterator, typename Condition_func>
Iterator find_closest_point(const Point& ref, Iterator begin, Iterator end, float accepting_distance, const Condition_func& cond);



template<typename Point, typename Iterator>
point_cloud_selection<Point> find_nearest_neighbors(const Point& ref, Iterator begin, Iterator end, std::size_t k);


/**
Sets all points to given color.
Iterator must yield point_full objects.
*/
template<typename Iterator>
void set_unique_color(Iterator begin, Iterator end, rgb_color);

template<typename Iterator>
void colorize_by_weight(Iterator begin, Iterator end, float min_w, float max_w, const rgb_color& col_min = rgb_color::black, const rgb_color& col_max = rgb_color::white);

template<typename Iterator>
void compute_lambertian_illumination_weights(Iterator begin, Iterator end, const Eigen::Vector3f& light_source);



/**
Normalize weights of given set of points.
Multiplies the points' weights by a constant factor such that they sum up to the number of points. Necessary for usage in some algorithms.
*/
template<typename Iterator>
void normalize_point_weights(Iterator begin, Iterator end);

template<typename Iterator>
void set_unique_weight(Iterator begin, Iterator end, std::uint8_t w);

template<typename Iterator>
void orient_normals_to_point(Iterator begin, Iterator end, const Eigen::Vector3f& ref, bool away = false);

template<typename Iterator>
void compute_normal_direction_weights(Iterator begin, Iterator end, const Eigen::Vector3f& ref, angle maximal_angle);

template<typename Iterator>
plane fit_plane_to_points(Iterator begin, Iterator end);

template<typename Iterator>
bounding_box compute_bounding_box(Iterator begin, Iterator end, float ep = 0.0);

template<typename Iterator>
Eigen::Vector3f center_of_mass(Iterator begin, Iterator end);

template<typename Iterator>
Eigen::Vector3f weighted_center_of_mass(Iterator begin, Iterator end);


template<typename Iterator>
plane compute_tangent_plane(const point_xyz&, Iterator neighbors_begin, Iterator neighbors_end);

template<typename Iterator>
float compute_local_surface_density(const point_full&, Iterator neighbors_begin, Iterator neighbors_end);



}

#include "point_algorithm.tcc"

#endif
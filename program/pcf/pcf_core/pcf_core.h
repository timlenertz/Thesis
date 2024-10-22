// File automatically generated by generate_pcf_core_h.py

#ifndef PCF_PCF_H_
#define PCF_PCF_H_

#include "point.h"
#include "point_algorithm.h"
#include "point_cloud_algorithm.h"
#include "point_full.h"
#include "point_xyz.h"
#include "rgb_color.h"
#include "space_object.h"
#include "space_object_fwd.h"
#include "space_object_observer.h"
#include "space_object_wrapper.h"
#include "artificial/artificial.h"
#include "artificial/disks.h"
#include "artificial/relief.h"
#include "camera/camera.h"
#include "camera/image_camera.h"
#include "camera/projection_camera.h"
#include "camera/projection_image_camera.h"
#include "camera/range_camera.h"
#include "camera/range_image_camera.h"
#include "field/field.h"
#include "field/gaussian_weight.h"
#include "field/list_field.h"
#include "geometry/angle.h"
#include "geometry/bounding_box.h"
#include "geometry/frustum.h"
#include "geometry/math_constants.h"
#include "geometry/plane.h"
#include "geometry/pose.h"
#include "geometry/projection_bounding_box.h"
#include "geometry/projection_frustum.h"
#include "geometry/spherical_coordinates.h"
#include "image/image.h"
#include "image/masked_image.h"
#include "image/projected_range_image.h"
#include "image/range_image.h"
#include "image/rgb_color_image.h"
#include "image/rgb_color_opencv.h"
#include "image/registration/mutual_information.h"
#include "io/ply_exporter.h"
#include "io/ply_importer.h"
#include "io/point_cloud_exporter.h"
#include "io/point_cloud_importer.h"
#include "io/pointscan_importer.h"
#include "io/range_point_cloud_importer.h"
#include "point_cloud/export.h"
#include "point_cloud/filter_point_cloud.h"
#include "point_cloud/import.h"
#include "point_cloud/merge.h"
#include "point_cloud/point_cloud.h"
#include "point_cloud/segment.h"
#include "point_cloud/segment_union.h"
#include "point_cloud/segment_union_iterator.h"
#include "point_cloud/selection.h"
#include "point_cloud/transform_iterator.h"
#include "point_cloud/grid/grid_point_cloud.h"
#include "point_cloud/grid/subspace.h"
#include "point_cloud/range/camera_range_point_cloud.h"
#include "point_cloud/range/range_point_cloud.h"
#include "point_cloud/tree/kdtree_traits.h"
#include "point_cloud/tree/node_handle.h"
#include "point_cloud/tree/null_tree_traits.h"
#include "point_cloud/tree/octree_traits.h"
#include "point_cloud/tree/tree_point_cloud.h"
#include "point_cloud/unorganized/unorganized_point_cloud.h"
#include "point_filter/accept.h"
#include "point_filter/conjunction.h"
#include "point_filter/crop.h"
#include "point_filter/disjunction.h"
#include "point_filter/negation.h"
#include "point_filter/point_filter.h"
#include "point_filter/probability.h"
#include "point_filter/reject.h"
#include "point_filter/segment.h"
#include "point_filter/skip.h"
#include "point_filter/weighted_probability.h"
#include "registration/iterative_correspondences_registration.h"
#include "registration/correspondences/closest_point_correspondences.h"
#include "registration/correspondences/registration_correspondence.h"
#include "registration/correspondences/registration_correspondences_list.h"
#include "registration/correspondences/same_point_correspondences.h"
#include "registration/error_metric/cross_histogram_error.h"
#include "registration/error_metric/mean_absolute_error.h"
#include "registration/error_metric/mean_square_error.h"
#include "registration/transformation_estimation/correlation_matrix_transformation_estimation.h"
#include "registration/transformation_estimation/svd_transformation_estimation.h"
#include "util/aligned_allocator.h"
#include "util/coordinates.h"
#include "util/default_allocator.h"
#include "util/dereference_iterator.h"
#include "util/io.h"
#include "util/memory.h"
#include "util/misc.h"
#include "util/mmap_allocator.h"
#include "util/multi_dimensional_array.h"
#include "util/multi_dimensional_buffer.h"
#include "util/random.h"

#endif

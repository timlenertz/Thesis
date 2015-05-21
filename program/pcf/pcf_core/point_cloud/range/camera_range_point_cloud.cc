#include "camera_range_point_cloud.h"

namespace pcf {

template class camera_range_point_cloud<point_xyz, projection_image_camera>;
template class camera_range_point_cloud<point_full, projection_image_camera>;

}
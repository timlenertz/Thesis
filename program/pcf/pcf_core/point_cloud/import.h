#ifndef PCF_POINT_CLOUD_IMPORT_H_
#define PCF_POINT_CLOUD_IMPORT_H_

#include "unorganized/unorganized_point_cloud.h"
#include "range/range_point_cloud.h"
#include <string>

namespace pcf {

/**
Import unorganized point cloud from file.
*/
unorganized_point_cloud_full import_point_cloud(const std::string&);

/**
Import range point cloud from file.
*/
range_point_cloud_full import_range_point_cloud(const std::string&);

}

#endif

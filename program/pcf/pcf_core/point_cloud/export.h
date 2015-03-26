#ifndef PCF_POINT_CLOUD_EXPORT_H_
#define PCF_POINT_CLOUD_EXPORT_H_

#include "point_cloud.h"
#include <string>

namespace pcf {

/**
Export unorganized point cloud to file.
*/
void export_point_cloud(const std::string&, const point_cloud_xyz&);

/**
Export unorganized point cloud to file.
*/
void export_point_cloud(const std::string&, const point_cloud_full&);

}

#endif

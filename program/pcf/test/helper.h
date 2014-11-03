#ifndef PCFTEST_HELPER_H_
#define PCFTEST_HELPER_H_

#include "../pcf/point_cloud/point_cloud.h"
#include "../pcf/point.h"

namespace pcf {

point_cloud<point_xyz> example_model(const char* name);

}

#endif
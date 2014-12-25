#ifndef PCFAPP_LIB_H_
#define PCFAPP_LIB_H_

#include "tui.h"
#include "../pcf/pcf.h"
#include "../pcf_viewer/pcf_viewer.h"

#include <string>

namespace pcf {

std::string file_name_extension(const std::string& filename);
std::string change_file_name_extension(const std::string& filename, const std::string& ext);

unorganized_point_cloud_full load_point_cloud(const std::string&);
range_point_cloud_full load_range_point_cloud(const std::string&);

unorganized_point_cloud_full ask_point_cloud();
range_point_cloud_full ask_range_point_cloud();

}

#endif
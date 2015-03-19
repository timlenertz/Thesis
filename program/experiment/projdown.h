#ifndef EX_PROJDOWN_H_
#define EX_PROJDOWN_H_

#include <pcf/core.h>
#include <pcf/experiment.h>
#include <string>

void projdown(const std::string& pc_name, const std::string& db_name, const pcf::pose& cam_pose, float circle_radius);

#endif
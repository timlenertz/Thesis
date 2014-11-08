#include "helper.h"
#include "../pcf/io/ply_reader.h"
#include <string>

namespace pcf {

point_cloud<point_xyz> example_model(const char* name) {
	ply_reader ply(std::string("../") + name + ".ply");
	return point_cloud<point_xyz>::create_from_reader(ply);
}

}
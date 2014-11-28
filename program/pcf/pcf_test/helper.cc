#include "helper.h"
#include "../pcf/io/ply_importer.h"
#include <string>

namespace pcf {

unorganized_point_cloud_xyz example_model(const char* name) {
	ply_importer ply(std::string("../") + name + ".ply");
	unorganized_point_cloud_xyz pc(ply);
	return pc;
}

}
#include "import.h"
#include "../io/ply_exporter.h"
#include "../util/misc.h"
#include <stdexcept>

namespace pcf {

void export_point_cloud(const std::string& path, const point_cloud_xyz& pc) {
	std::string ext = file_name_extension(path);
	if(ext == "ply") {
		ply_exporter ex(path, false);
		pc.export_with(ex);
	} else {
		throw std::invalid_argument("Unknown point cloud file name extension: " + ext);
	}
}

void export_point_cloud(const std::string& path, const point_cloud_full& pc) {
	std::string ext = file_name_extension(path);
	if(ext == "ply") {
		ply_exporter ex(path, true);
		pc.export_with(ex);
	} else {
		throw std::invalid_argument("Unknown point cloud file name extension: " + ext);
	}
}

}
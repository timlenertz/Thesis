#include "import.h"
#include "../io/ply_importer.h"
#include "../io/pointscan_importer.h"
#include "../util/misc.h"
#include <stdexcept>

namespace pcf {

unorganized_point_cloud_full import_point_cloud(const std::string& path) {
	std::string ext = file_name_extension(path);
	if(ext == "ply") {
		ply_importer imp(path);
		unorganized_point_cloud_full pc(imp);
		pc.set_name(path);
		return pc;
	} else if(ext == "scan") {
		pointscan_importer imp(path);
		unorganized_point_cloud_full pc(imp);
		pc.set_name(path);
		return pc;
	} else {
		throw std::invalid_argument("Unknown point cloud file name extension: " + ext);
	}
}


range_point_cloud_full import_range_point_cloud(const std::string& path) {
	std::string ext = file_name_extension(path);
	if(ext == "scan") {
		pointscan_importer imp(path);
		range_point_cloud_full pc(imp);
		pc.set_name(path);
		return pc;
	} else {
		throw std::invalid_argument("Unknown range point cloud file name extension: " + ext);
	}
}

}
#include "point_cloud_importer.h"
#include <stdexcept>

namespace pcf {

bool point_cloud_importer::has_camera_pose() const {
	return false;
}

pose point_cloud_importer::camera_pose() const {
	throw std::logic_error("Importer does not provide camera pose information.");
}


}
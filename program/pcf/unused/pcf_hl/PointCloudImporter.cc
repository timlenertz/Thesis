#include "PointCloudImporter.h"

#include "../pcf/util/misc.h"
#include "../pcf/io/ply_importer.h"
#include "../pcf/io/pointscan_importer.h"
#include <stdexcept>
#include <string>
#include <memory>

namespace pcf {

void PointCloudImporter::run() {
	std::unique_ptr<point_cloud_importer> importer;
	
	std::string ext = file_name_extension(file_name_);
	if(ext == "ply") importer.reset( new ply_importer(file_name_) );
	else if(ext == "scan") importer.reset( new pointscan_importer(file_name_) );
	
	if(importer) {
		auto* pc = new unorganized_point_cloud_full(*importer);
		output_.reset(pc);
	} else {
		throw std::runtime_error("Unknown point cloud file extension ." ext);
	}
}


}
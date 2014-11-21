#include "range_point_cloud_importer.h"

namespace pcf {

std::size_t range_point_cloud_importer::size() const {
	return this->width() * this->height();
}


bool range_point_cloud_importer::all_valid() const {
	return false;
}


void range_point_cloud_importer::read_row(point_xyz* buf) {
	this->read(buf, this->width());
}


void range_point_cloud_importer::read_row(point_full* buf) {
	this->read(buf, this->width());
}



}

#include "range_point_cloud_importer.h"

namespace pcf {

std::size_t range_point_cloud_importer::size() const {
	return this->rows() * this->columns();
}


bool range_point_cloud_importer::all_valid() const {
	return false;
}


void range_point_cloud_importer::read_row(point_xyz* buf) {
	this->read(buf, this->columns());
}


void range_point_cloud_importer::read_row(point_full* buf) {
	this->read(buf, this->columns());
}


void range_point_cloud_importer::read_row_spherical(spherical_coordinates* buf) {
	this->read_spherical(buf, this->columns());
}


std::unique_ptr<image_camera> range_point_cloud_importer::estimate_camera() const {
	return std::unique_ptr<image_camera>();
}


}

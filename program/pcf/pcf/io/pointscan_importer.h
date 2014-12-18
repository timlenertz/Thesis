#ifndef PCF_POINTSCAN_IMPORTER_H_
#define PCF_POINTSCAN_IMPORTER_H_

#include "range_point_cloud_importer.h"
#include <memory>
#include <fstream>
#include <string>

namespace ps {
	struct sc_scan;
};

namespace pcf {

/**
Imports range point cloud from .scan pointscan file.
Based on external/pointscan/core.cpp code. Handles reading of cartesian, spherical coordinates, color information and normal vectors from file. Does not copy data into internal buffers. If no cartesian coordinates are in file, reads and transforms the spherical coordinates.
*/
class pointscan_importer : public range_point_cloud_importer {
private:
	struct sc_header;

	std::ifstream file_;
	std::unique_ptr<sc_header> header_;
	std::ptrdiff_t current_point_position_;
	
	std::ifstream::pos_type cartesian_coordinates_offset_;
	std::ifstream::pos_type spherical_coordinates_offset_;
	std::ifstream::pos_type color_offset_;
	std::ifstream::pos_type normal_offset_;
	
	void seek_to_cartesian_coordinates_();
	void seek_to_spherical_coordinates_();
	void seek_to_color_();
	void seek_to_normal_();
	
public:
	explicit pointscan_importer(const std::string& filename);
	~pointscan_importer();

	bool has_camera_pose() const override;
	pose camera_pose() const override;

	std::size_t rows() const override;
	std::size_t columns() const override;
	
	void rewind() override;
	std::ptrdiff_t tell() const override;
	void read(point_xyz*, std::size_t sz) override;
	void read(point_full*, std::size_t sz) override;
	void read_spherical(spherical_coordinates*, std::size_t sz) override;
	
	bool has_color() const { return color_offset_; }
	bool has_normal() const { return normal_offset_; }
	bool has_cartesian_coordinates() const { return cartesian_coordinates_offset_; }
	bool has_spherical_coordinates() const { return spherical_coordinates_offset_; }
};

}

#endif
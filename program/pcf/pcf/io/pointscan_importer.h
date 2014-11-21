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

class pointscan_importer : public range_point_cloud_importer {
private:
	struct sc_header;

	std::ifstream file_;
	std::unique_ptr<sc_header> header_;
	std::ptrdiff_t current_point_position_;
	
	std::ifstream::pos_type coordinates_offset_;
	bool cartesian_coordinates_;
	std::ifstream::pos_type color_offset_;
	std::ifstream::pos_type normal_offset_;

public:
	explicit pointscan_importer(const std::string& filename);

	~pointscan_importer();

	std::size_t width() const override;
	std::size_t height() const override;
	
	void rewind() override;
	std::ptrdiff_t tell() const override;
	void read(point_xyz*, std::size_t sz) override;
	void read(point_full*, std::size_t sz) override;
	
	bool has_color() const { return color_offset_; }
	bool has_normal() const { return normal_offset_; }
};

}

#endif
#ifndef PCF_POINTSCAN_IMPORTER_H_
#define PCF_POINTSCAN_IMPORTER_H_

#include "range_point_cloud_importer.h"
#include <memory>
#include <istream>
#include <string>

namespace ps {
	struct sc_scan;
};

namespace pcf {

class pointscan_importer : public range_point_cloud_importer {
private:
	struct ps_data;

	std::ifstream file_;
	std::unique_ptr<>
	std::ifstream::pos_type data_set_starts_[10];
	std::ifstream::pos_type data_set_positions_[10];
	std::ptrdiff_t current_point_position_;

public:
	explicit pointscan_importer(const char* filename);
	explicit pointscan_importer(const std::string& filename) :
		pointscan_importer(filename.c_str()) { }

	std::size_t width() const override;
	std::size_t height() const override;
	
	void rewind() override;
	std::ptrdiff_t tell() const override;
	void read(point_xyz*, std::size_t sz) override;
	void read(point_full*, std::size_t sz) override;
};

}

#endif
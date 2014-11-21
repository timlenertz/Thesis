#include "point_cloud_exporter.h"
#include <fstream>
#include <string>
#include "../point.h"
#include "../util/io.h"

namespace pcf {

class ply_exporter : public point_cloud_exporter {
private:
	const static std::size_t vertex_count_string_length_ = 15;

	std::ofstream file_;
	std::ofstream::pos_type vertex_count_string_position_;
	std::size_t count_ = 0;
	
	const line_delimitor line_delimitor_;
	const bool full_;
	const bool ascii_;
		
	void write_line_(const std::string& ln);
	
	void write_binary_(const point_xyz&);
	void write_ascii_(const point_xyz&);
	void write_binary_(const point_full&);
	void write_ascii_(const point_full&);


public:
	explicit ply_exporter(const std::string& filename, bool full = true, bool ascii = false, line_delimitor ld = line_delimitor::LF);
	~ply_exporter();
	
	void write(const point_xyz*, std::size_t sz) override final;
	void write(const point_full*, std::size_t sz) override final;
	void close() override final;
};

}
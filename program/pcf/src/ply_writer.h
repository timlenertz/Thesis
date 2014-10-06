#ifndef PCF_PLY_WRITER_H_
#define PCF_PLY_WRITER_H_

#include <fstream>
#include <string>

#include "util.h"

namespace pcf {

class ply_writer_base {
protected:
	constexpr static std::size_t vertex_count_string_length_ = 15;
	constexpr static line_delimitor line_delimitor_ = line_delimitor::CR;

	std::ofstream file_;
	std::ifstream::pos_type vertex_count_string_position_;
	std::ofstream::pos_type vertex_data_start_;
	std::size_t count_ = 0;
	
	explicit ply_writer_base(const std::string& path);
	virtual ~ply_writer_base() { }
	
	virtual void write_vertex_properties_definition_() { }
	
	void write_header_();
	void write_count_();
};

}

#endif
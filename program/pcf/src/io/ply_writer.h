#ifndef PCF_PLY_WRITER_H_
#define PCF_PLY_WRITER_H_

#include <fstream>
#include <string>

#include "../point.h"
#include "util.h"

namespace pcf {

class ply_writer_base {
protected:
	constexpr static std::size_t vertex_count_string_length_ = 15;
	constexpr static line_delimitor line_delimitor_ = line_delimitor::LF;

	std::ofstream file_;
	std::ofstream::pos_type vertex_count_string_position_;
	std::ofstream::pos_type vertex_data_start_;
	std::size_t count_ = 0;
	bool header_written_ = false;
		
	virtual void write_vertex_properties_definition_() = 0;
	
	void write_line_(const std::string& ln) {
		write_line(file_, ln, line_delimitor_);
	}
	
	void write_header_();
	void overwrite_count_(bool seek_back = true);
	
	void write_raw_(const char* buffer, std::streamsize sz) {
		if(! header_written_) write_header_();
		file_.write(buffer, sz);
	}
	
public:
	explicit ply_writer_base(const std::string& path);
	virtual ~ply_writer_base() { }
	
	std::size_t size() const { return count_; }
};



template<typename Point> class ply_writer;


template<>
class ply_writer<point_xyz> : public ply_writer_base {
public:
	using ply_writer_base::ply_writer_base;
	
	void write_vertex_properties_definition_() override {
		write_line_("property float x");
		write_line_("property float y");
		write_line_("property float z");
	}
	
	void write(const point_xyz* buffer, std::size_t n) {
		count_ += n;
		while(n--)
			write_raw_(reinterpret_cast<const char*>( &(buffer++)->homogeneous_coordinates ), 3 * sizeof(float));
		overwrite_count_();
	}
};


template<>
class ply_writer<point_full> : public ply_writer_base {
public:
	using ply_writer_base::ply_writer_base;
	
	void write_vertex_properties_definition_() override {
		write_line_("property float x");
		write_line_("property float y");
		write_line_("property float z");
		write_line_("property float nx");
		write_line_("property float ny");
		write_line_("property float nz");
		write_line_("property uchar red");
		write_line_("property uchar green");
		write_line_("property uchar blue");
	}
	
	void write(const point_full* buffer, std::size_t n) {
		count_ += n;
		while(n--) {
			write_raw_(reinterpret_cast<const char*>( &buffer->homogeneous_coordinates ), 3 * sizeof(float));
			write_raw_(reinterpret_cast<const char*>( &buffer->normal ), 3 * sizeof(float));
			write_raw_(reinterpret_cast<const char*>( &buffer->color ), 3);
			++buffer;
		}
		overwrite_count_();
	}
};

}

#endif
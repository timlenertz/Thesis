#ifndef PCF_PLY_WRITER_H_
#define PCF_PLY_WRITER_H_

#include <fstream>
#include <string>
#include <cassert>
#include "../point.h"
#include "../util/io.h"

namespace pcf {

class ply_writer_base {
protected:
	const static std::size_t vertex_count_string_length_ = 15;
	const static line_delimitor line_delimitor_ = line_delimitor::LF;

	std::ofstream file_;
	std::ofstream::pos_type vertex_count_string_position_;
	std::ofstream::pos_type vertex_data_start_;
	std::size_t count_ = 0;
	bool header_written_ = false;
	const bool ascii_;
	
	virtual void write_vertex_properties_definition_() = 0;
	
	void write_line_(const std::string& ln) {
		write_line(file_, ln, line_delimitor_);
	}
	
	void write_header_();
	void overwrite_count_(bool seek_back = true);
	
public:
	explicit ply_writer_base(const std::string& path, bool ascii);
	virtual ~ply_writer_base() { }
	
	std::size_t size() const { return count_; }
};



template<typename Point> class ply_writer;


template<>
class ply_writer<point_xyz> : public ply_writer_base {
private:
	void write_binary_(const point_xyz&);
	void write_ascii_(const point_xyz&);

public:
	explicit ply_writer(const std::string& path, bool ascii = false) : ply_writer_base(path, ascii) { }
	
	void write_vertex_properties_definition_() override;
	void write(const point_xyz* buffer, std::size_t n);
};



template<>
class ply_writer<point_full> : public ply_writer_base {
private:
	void write_binary_(const point_full&);
	void write_ascii_(const point_full&);

public:
	explicit ply_writer(const std::string& path, bool ascii = false) : ply_writer_base(path, ascii) { }
	
	void write_vertex_properties_definition_() override;
	void write(const point_full* buffer, std::size_t n);
};

}

#endif
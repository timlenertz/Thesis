#ifndef PCF_PLY_READER_H_
#define PCF_PLY_READER_H_

#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <array>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <utility>
#include <memory>
#include <cctype>

#include "../point.h"
#include "../util/io.h"

namespace pcf {

/**
Point cloud reader from PLY files.
Reads only points (vertices), possibly with RGB color and normal vector data. Supports ASCII and binary
formats. Detects line ending type from file. File may contains other elements except vertex, but those
are not read. List-type properties are not supported, and are only tolerated in elements behinds vertex.
*/
class ply_reader {
private:
	const static std::size_t maximal_ascii_element_line_length_ = 256;

	enum property_type {
		none, int8, uint8, int16, uint16, int32, uint32, float32, float64, list
	};

	struct property {
		property_type type = none;
		std::ptrdiff_t offset;
		std::ptrdiff_t index;
		
		explicit operator bool() const { return (type != none); }
	};
		
	std::ifstream file_;
	line_delimitor line_delimitor_;
	std::ifstream::pos_type vertex_data_start_; ///< File offset where vertex data starts.
	std::size_t number_of_vertices_; ///< Number of vertex elements in file.
	enum { binary_big_endian, binary_little_endian, ascii } format_; ///< Format of file.
	std::size_t vertex_length_; ///< For binary formats, length of one vertex element.
	std::size_t number_of_properties_; ///< Number of properties for vertex element.
	bool has_rgb_; ///< Whether r_, g_, b_ are defined.
	property x_, y_, z_, r_, g_, b_, nx_, ny_, nz_; ///< Offsets, Indices and types of vertex properties.
	
	std::ptrdiff_t current_element_; ///< Index of current element.


	void skip_lines_(std::size_t n) {
		while(n--) skip_line(file_, line_delimitor_);
	}
	
	void read_line_(std::string& ln) {
		read_line(file_, ln, line_delimitor_);
	}
	
	bool is_host_endian_binary_() const {
		return (format_ == host_is_little_endian ? binary_little_endian : binary_big_endian);
	}
	
	property* identify_property_(const std::string& nm);
	static property_type identify_property_type_(const std::string& nm);
	static std::size_t property_type_size_(property_type t);
	void read_header_();
	
	template<typename Point> void read_ascii_(Point* buffer, std::size_t n);
	template<typename Point> void read_binary_(Point* buffer, std::size_t n);
	
	void read_ascii_point_(point_xyz& out_point, const char* props[]) const;
	void read_ascii_point_(point_full& out_point, const char* props[]) const;
	void read_binary_point_(point_xyz& out_point, char* data) const;
	void read_binary_point_(point_full& out_point, char* data) const;
	template<typename T> T read_binary_property_(const property& prop, char* data) const;
	
public:
	explicit ply_reader(const char* filename, line_delimitor ld = line_delimitor::unknown);
	explicit ply_reader(const std::string& filename, line_delimitor ld = line_delimitor::unknown) :
		ply_reader(filename.c_str(), ld) { }
	
	std::size_t size() const { return number_of_vertices_; }
	bool all_valid() const { return true; }
	
	bool is_binary() const { return format_ != ascii; }
	bool is_ascii() const { return format_ == ascii; }
	
	std::ptrdiff_t tell() const { return number_of_vertices_; }
	void rewind();
	template<typename Point> void read(Point* buffer, std::size_t n);
};

}

#include "ply_reader.tcc"

#endif
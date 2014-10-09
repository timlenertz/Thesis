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

#include "../point.h"
#include "util.h"

namespace pcf {


class ply_reader_error : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};


class ply_reader {
private:
	constexpr static std::size_t maximal_ascii_element_line_length_ = 256;

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
	property x_, y_, z_, r_, g_, b_; ///< Offsets, Indices and types of vertex properties.
	
	std::ptrdiff_t current_element_; ///< Index of current element.


	void skip_lines_(std::size_t n) {
		while(n--) skip_line(file_, line_delimitor_);
	}
	
	void read_line_(std::string& ln) {
		read_line(file_, ln, line_delimitor_);
	}
	
	bool is_host_endian_binary_() const { return (format_ == host_is_little_endian() ? binary_little_endian : binary_big_endian); }
	
	property* identify_property_(const std::string& nm);
	static property_type identify_property_type_(const std::string& nm);
	static std::size_t property_type_size_(property_type t);
	void read_header_();
	
	template<typename Point> void read_ascii_(Point* buffer, std::size_t n);
	template<typename Point> void read_binary_(Point* buffer, std::size_t n);
	
	void read_ascii_point_(point_xyz& out_point, const char* props[]) const;
	//void read_ascii_point_(point_xyzrgb& out_point, const char* props[]) const;
	void read_binary_point_(point_xyz& out_point, char* data) const;
	//void read_binary_point_(point_xyzrgb& out_point, char* data) const;
	template<typename T> T read_binary_property_(const property& prop, char* data) const;
	
public:
	explicit ply_reader(const char* filename, line_delimitor ld = line_delimitor::unknown);
	
	std::size_t size() const { return number_of_vertices_; }
	bool is_binary() const { return format_ != ascii; }
	bool is_ascii() const { return format_ == ascii; }
	
	std::ptrdiff_t tell() const { return number_of_vertices_; }
	void rewind();
	template<typename Point> void read(Point* buffer, std::size_t n);
};



template<typename Point>
void ply_reader::read(Point* buffer, std::size_t n) {
	if(current_element_ + n > number_of_vertices_) throw ply_reader_error("Attempted to read beyond bounds.");
	
	if(is_ascii()) read_ascii_(buffer, n);
	else read_binary_(buffer, n);
	
	current_element_ += n;
}


template<typename Point>
void ply_reader::read_ascii_(Point* buffer, std::size_t n) {
	std::string line;
	std::vector<const char*> properties(number_of_properties_, nullptr);
	
	while(n--) {
		read_line_(line);
				
		auto property = properties.begin();
		
		bool last_space = true;
		for(const char& c : line) {
			bool space = std::isspace(c);
			if(last_space == space) continue;
			else if(last_space) *(property++) = &c;
			last_space = space;
		}
		
		read_ascii_point_(*buffer, properties.data());
		++buffer;
	}
}


template<typename Point>
void ply_reader::read_binary_(Point* out, std::size_t n) {
	std::streamsize length = n * vertex_length_;
	std::unique_ptr<char[]> buffer(new char[length]);
	file_.read(buffer.get(), length);
	
	char* buffer_end = buffer.get() + length;
	for(char* in = buffer.get(); in != buffer_end; in += vertex_length_, ++out)
		read_binary_point_(*out, in);
}


template<typename T>
T ply_reader::read_binary_property_(const property& prop, char* data) const {
	std::size_t sz = property_type_size_(prop.type);
	data += prop.offset;
	if(! is_host_endian_binary_()) flip_endianness(data, sz);
	switch(prop.type) {
		case int8:   return static_cast<T>( *reinterpret_cast<const std::int8_t*>(data) );
		case uint8:  return static_cast<T>( *reinterpret_cast<const std::uint8_t*>(data) );
		case int16:  return static_cast<T>( *reinterpret_cast<const std::int16_t*>(data) );
		case uint16: return static_cast<T>( *reinterpret_cast<const std::uint16_t*>(data) );
		case int32:  return static_cast<T>( *reinterpret_cast<const std::int32_t*>(data) );
		case uint32: return static_cast<T>( *reinterpret_cast<const std::uint32_t*>(data) );
		default: break;
	}
	if(host_has_iec559_float) switch(prop.type) {
		case float32: return static_cast<T>( *reinterpret_cast<const float*>(data) );
		case float64: return static_cast<T>( *reinterpret_cast<const double*>(data) );
		default: break;
	}
	return T();
}



}

#endif
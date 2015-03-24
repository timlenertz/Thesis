#include "ply_writer.h"

#include <iomanip>
#include <stdexcept>

#include <iostream>

namespace pcf {

ply_writer_base::ply_writer_base(const std::string& path, bool ascii) :
file_(path, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary), ascii_(ascii) {
	// Need to defer writing header until subclass is initialized.
	header_written_ = false;
}

void ply_writer_base::write_header_() {
	if(header_written_) throw std::logic_error("PLY header already written.");
	header_written_ = true;

	write_line_("ply");
	if(ascii_) write_line_("format ascii 1.0");
	else if(host_is_little_endian) write_line_("format binary_little_endian 1.0");
	else write_line_("format binary_big_endian 1.0");
	
	write_line_("comment PLY file generated using pcf::ply_writer");
	
	file_ << "element vertex ";
	file_ << std::flush;
	vertex_count_string_position_ = file_.tellp();
	overwrite_count_(false);
	write_line_("");
	
	this->write_vertex_properties_definition_();
	
	write_line_("end_header");
	file_ << std::flush;
	vertex_data_start_ = file_.tellp();
	
}

void ply_writer_base::overwrite_count_(bool seek_back) {
	if(! header_written_) throw std::logic_error("PLY header not yet written.");

	std::ofstream::pos_type old_position;
	if(seek_back) old_position = file_.tellp();
	
	file_.seekp(vertex_count_string_position_);
	file_ << std::setfill(' ') << std::left << std::setw(vertex_count_string_length_) << count_;
	file_ << std::flush;
		
	if(seek_back) file_.seekp(old_position);
}


void ply_writer<point_xyz>::write_vertex_properties_definition_() {
	write_line_("property float x");
	write_line_("property float y");
	write_line_("property float z");
}


void ply_writer<point_xyz>::write(const point_xyz* buffer, std::size_t n) {
	if(! header_written_) write_header_();	
	count_ += n;
	while(n--) {
		assert(buffer->valid());
		if(ascii_) write_ascii_(*buffer);
		else write_binary_(*buffer);
		++buffer;	
	}
	overwrite_count_();
}


void ply_writer<point_xyz>::write_binary_(const point_xyz& p) {
	file_.write(reinterpret_cast<const char*>( p.data() ), 3 * sizeof(float));
}


void ply_writer<point_xyz>::write_ascii_(const point_xyz& p) {
	file_ << p[0] << ' ' << p[1] << ' ' << p[2];
	end_line(file_, line_delimitor_);
}



void ply_writer<point_full>::write_vertex_properties_definition_() {
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


void ply_writer<point_full>::write(const point_full* buffer, std::size_t n) {
	if(! header_written_) write_header_();	
	count_ += n;
	while(n--) {
		assert(buffer->valid());
		if(ascii_) write_ascii_(*buffer);
		else write_binary_(*buffer);
		++buffer;	
	}
	overwrite_count_();
}


void ply_writer<point_full>::write_binary_(const point_full& p) {
	file_.write(reinterpret_cast<const char*>( p.data() ), 3 * sizeof(float));
	file_.write(reinterpret_cast<const char*>( p.normal.data() ), 3 * sizeof(float));
	file_.write(reinterpret_cast<const char*>( &p.color ), 3);
}


void ply_writer<point_full>::write_ascii_(const point_full& p) {
	rgb_color col = 
	file_ << p[0] << ' ' << p[1] << ' ' << p[2]
		<< ' ' << p.normal[0] << ' ' << p.normal[1] << ' ' << p.normal[2]
		<< ' ' << (unsigned)p.color.r << ' ' << (unsigned)p.color.g << ' ' << (unsigned)p.color.b;
	end_line(file_, line_delimitor_);
}


}
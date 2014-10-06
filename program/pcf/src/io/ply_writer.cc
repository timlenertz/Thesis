#include "ply_writer.h"

#include <iomanip>
#include <stdexcept>

#include <iostream>

namespace pcf {

ply_writer_base::ply_writer_base(const std::string& path) :
file_(path, std::ios_base::out | std::ios_base::trunc) {
	// Need to defer writing header until subclass is initialized.
	header_written_ = false;
}

void ply_writer_base::write_header_() {
	if(header_written_) throw std::logic_error("PLY header already written.");
	header_written_ = true;

	write_line_("ply");
	if(host_is_little_endian()) write_line_("format binary_little_endian 1.0");
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




}
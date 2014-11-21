#include "ply_exporter.h"
#include <iomanip>
#include <stdexcept>
#include <cassert>


namespace pcf {

ply_exporter::ply_exporter(const std::string& filename, bool full, bool ascii, line_delimitor ld) :
	file_(filename, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary),
	line_delimitor_(ld),
	full_(full),
	ascii_(ascii)
{
	write_line_("ply");
	if(ascii_) write_line_("format ascii 1.0");
	else if(host_is_little_endian) write_line_("format binary_little_endian 1.0");
	else write_line_("format binary_big_endian 1.0");
	
	write_line_("comment PLY file generated using pcf::ply_writer");
	
	file_ << "element vertex ";
	file_ << std::flush;
	vertex_count_string_position_ = file_.tellp();
	file_ << std::setfill(' ') << std::left << std::setw(vertex_count_string_length_) << 0;
	write_line_("");
	
	write_line_("property float x");
	write_line_("property float y");
	write_line_("property float z");
	if(full_) {
		write_line_("property float nx");
		write_line_("property float ny");
		write_line_("property float nz");
		write_line_("property uchar red");
		write_line_("property uchar green");
		write_line_("property uchar blue");
	}
	
	write_line_("end_header");
}


ply_exporter::~ply_exporter() {
	close();
}


void ply_exporter::close() {
	// Write vertex count to file
	file_.seekp(vertex_count_string_position_);
	file_ << std::setfill(' ') << std::left << std::setw(vertex_count_string_length_) << count_;

	// Close
	file_.close();
}


void ply_exporter::write(const point_xyz* buf, std::size_t sz) {
	count_ += sz;
	if(full_) {
		while(sz--) {
			assert(buffer->valid());
			if(ascii_) write_ascii_(point_full(*buf));
			else write_binary_(point_full(*buf));
			++buf;	
		}
	} else {
		while(sz--) {
			assert(buffer->valid());
			if(ascii_) write_ascii_(*buf);
			else write_binary_(*buf);
			++buf;
		}
	}

}


void ply_exporter::write(const point_full* buf, std::size_t sz) {
	count_ += sz;
	if(full_) {
		while(sz--) {
			assert(buffer->valid());
			if(ascii_) write_ascii_(*buf);
			else write_binary_(*buf);
			++buf;	
		}
	} else {
		while(sz--) {
			assert(buffer->valid());
			if(ascii_) write_ascii_(*(point_xyz*)buf);
			else write_binary_(*(point_xyz*)buf);
			++buf;
		}
	}
}


void ply_exporter::write_line_(const std::string& ln) {
	write_line(file_, ln, line_delimitor_);
}


void ply_exporter::write_binary_(const point_xyz& p) {
	file_.write(reinterpret_cast<const char*>( p.data() ), 3 * sizeof(float));
}


void ply_exporter::write_ascii_(const point_xyz& p) {
	file_ << p[0] << ' ' << p[1] << ' ' << p[2];
	end_line(file_, line_delimitor_);
}


void ply_exporter::write_binary_(const point_full& p) {
	file_.write(reinterpret_cast<const char*>( p.data() ), 3 * sizeof(float));
	file_.write(reinterpret_cast<const char*>( p.normal.data() ), 3 * sizeof(float));
	file_.write(reinterpret_cast<const char*>( &p.color ), 3);
}


void ply_exporter::write_ascii_(const point_full& p) {
	file_ << p[0] << ' ' << p[1] << ' ' << p[2]
		<< ' ' << p.normal[0] << ' ' << p.normal[1] << ' ' << p.normal[2]
		<< ' ' << (unsigned)p.color.r << ' ' << (unsigned)p.color.g << ' ' << (unsigned)p.color.b;
	end_line(file_, line_delimitor_);
}



}
#include "pointscan_importer.h"
#include "../util/io.h"
#include "../geometry/spherical_coordinates.h"
#include "../geometry/math_constants.h"
#include <cstdint>
#include <stdexcept>
#include <Eigen/Eigen>

namespace pcf {

namespace {	
	enum class sc_data_type : std::uint32_t {
		none = 0x0000,
		valididy = 0x0001,
		polar = 0x0002,
		amplitude = 0x0004,
		color = 0x0008,
		cartesian = 0x0010,
		normal = 0x0020,
		texture = 0x0040,
		intensity = 0x0080,
		intensity_corrected = 0x0100,
		all = 0x01ff
	};

	using sc_amplitude = float;
	using sc_cartesian = float[3];
	using sc_color = std::uint8_t[3];
	using sc_normal = float[3];
	using sc_polar = float[3];
	using sc_texture = std::uint32_t[3];
	using sc_validity = std::uint32_t;
	
	std::size_t element_size(sc_data_type type) {
		switch(type) {
		case sc_data_type::valididy:
			return sizeof(sc_validity);
		case sc_data_type::polar:
			return sizeof(sc_polar);
		case sc_data_type::color:
			return sizeof(sc_color);
		case sc_data_type::cartesian:
			return sizeof(sc_cartesian);
		case sc_data_type::normal:
			return sizeof(sc_normal);
		case sc_data_type::texture:
			return sizeof(sc_texture);
		case sc_data_type::amplitude: 
		case sc_data_type::intensity:
		case sc_data_type::intensity_corrected:
			return sizeof(sc_amplitude);
		default:
			throw std::invalid_argument("No element size for this type.");
		}
	}
	
	inline void read_polar(spherical_coordinates& s, const sc_polar& data) {
		if(data[0] == 0 && data[1] == 0 && data[2] == 0) {
			s.invalidate();
		} else {
			float range = data[0], phi = data[1], theta = data[2];	
			s = spherical_coordinates(range, phi, theta);
		}
	}
	
	inline void read_polar(point_xyz& pt, const sc_polar& data) {
		spherical_coordinates s;
		read_polar(s, data);
		if(s.valid()) pt = s.to_cartesian();
		else pt.invalidate();
	}
	
	inline void read_cartesian(point_xyz& pt, const sc_cartesian& data) {
		if(data[0] == 0 && data[1] == 0 && data[2] == 0) pt.invalidate();
		else pt = point_xyz(data[0], data[1], data[2]);
	}
	
	inline void read_color(point_full& pt, const sc_color& data) {
		pt.color.r = data[0];
		pt.color.g = data[1];
		pt.color.b = data[2];
	}
}


struct pointscan_importer::sc_header {
	std::int32_t columns;
	std::int32_t rows;
	std::uint32_t number_of_data_sets;
	sc_data_type subsequent_data_set_types[10];
	float pose[16];
};


// Needed because sc_header is incomplete in header
pointscan_importer::~pointscan_importer() { }



pointscan_importer::pointscan_importer(const std::string& filename) :
	file_(filename, std::ios_base::in | std::ios_base::binary),
	header_(new sc_header),
	current_point_position_(0)
{
	if(! host_is_little_endian)
		throw std::runtime_error("Need to be little endian to read pointscan files.");

	// Read header
	file_.read( (char*) header_.get(), sizeof(sc_header) );
	std::size_t total_count = header_->rows * header_->columns;

	// Look for data set types, and determine data set offsets
	cartesian_coordinates_offset_ = 0;
	spherical_coordinates_offset_ = 0;
	color_offset_ = 0;
	normal_offset_ = 0;
	
	std::ifstream::pos_type offset = file_.tellg();
	for(std::ptrdiff_t i = 0; i < header_->number_of_data_sets; ++i) {
		sc_data_type type = header_->subsequent_data_set_types[i];
		if(type == sc_data_type::cartesian) cartesian_coordinates_offset_ = offset;
		else if(type == sc_data_type::polar) spherical_coordinates_offset_ = offset;
		else if(type == sc_data_type::color) color_offset_ = offset;
		else if(type == sc_data_type::normal) normal_offset_ = offset;
				
		std::size_t sz = element_size(type);
		offset += total_count * sz;
	}
	
	if(!cartesian_coordinates_offset_ && !spherical_coordinates_offset_)
		throw std::runtime_error("No cartesian or spherical coordinates in pointscan file.");
}



void pointscan_importer::seek_to_cartesian_coordinates_() {
	std::ifstream::off_type off = current_point_position_ * sizeof(sc_cartesian);
	file_.seekg(cartesian_coordinates_offset_ + off);	
}


void pointscan_importer::seek_to_spherical_coordinates_() {
	std::ifstream::off_type off = current_point_position_ * sizeof(sc_polar);
	file_.seekg(spherical_coordinates_offset_ + off);	
}


void pointscan_importer::seek_to_color_() {
	std::ifstream::off_type off = current_point_position_ * sizeof(sc_color);
	file_.seekg(color_offset_ + off);	
}


void pointscan_importer::seek_to_normal_() {
	std::ifstream::off_type off = current_point_position_ * sizeof(sc_normal);
	file_.seekg(normal_offset_ + off);	
}


std::size_t pointscan_importer::rows() const {
	return header_->rows;
}


std::size_t pointscan_importer::columns() const {
	return header_->columns;
}


void pointscan_importer::rewind() {
	current_point_position_ = 0;
}


std::ptrdiff_t pointscan_importer::tell() const {
	return current_point_position_;
}


void pointscan_importer::read_spherical(spherical_coordinates* buf, std::size_t sz) {
	if(! spherical_coordinates_offset_)
		throw std::logic_error("No spherical coordinates in pointscan file.");
	
	spherical_coordinates* buf_end = buf + sz;
	seek_to_spherical_coordinates_();
	for(spherical_coordinates* s = buf; s != buf_end; ++s) {
		sc_polar data;
		file_.read( (char*)data, sizeof(sc_polar) );
		read_polar(*s, data);
	}
	
	current_point_position_ += sz;
}



void pointscan_importer::read(point_xyz* buf, std::size_t sz) {
	point_xyz* buf_end = buf + sz;

	if(cartesian_coordinates_offset_) {
		// Read cartesian coordinates from file
		seek_to_cartesian_coordinates_();
		for(point_xyz* p = buf; p != buf_end; ++p) {
			sc_cartesian data;
			file_.read( (char*)data, sizeof(sc_cartesian) );
			read_cartesian(*p, data);
		}
	} else {
		// No cartesian coordinates, instead read and transform spherical coordinates
		seek_to_spherical_coordinates_();
		for(point_xyz* p = buf; p != buf_end; ++p) {
			sc_polar data;
			file_.read( (char*)data, sizeof(sc_polar) );
			read_polar(*p, data);
		}	
	}
	
	current_point_position_ += sz;
}


void pointscan_importer::read(point_full* buf, std::size_t sz) {
	point_full* buf_end = buf + sz;

	if(cartesian_coordinates_offset_) {
		// Read cartesian coordinates from file
		seek_to_cartesian_coordinates_();
		for(point_full* p = buf; p != buf_end; ++p) {
			sc_cartesian data;
			file_.read( (char*)data, sizeof(sc_cartesian) );
			read_cartesian(*p, data);
		}
	} else {
		// No cartesian coordinates, instead read and transform spherical coordinates
		seek_to_spherical_coordinates_();
		for(point_full* p = buf; p != buf_end; ++p) {
			sc_polar data;
			file_.read( (char*)data, sizeof(sc_polar) );
			read_polar(*p, data);
		}	
	}
		
	if(color_offset_) {
		seek_to_color_();
		for(point_full* p = buf; p != buf_end; ++p) {
			sc_color data;
			file_.read( (char*)data, sizeof(sc_color) );
			read_color(*p, data);
		}
	}
	
	current_point_position_ += sz;
}


}
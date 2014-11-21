#include "pointscan_importer.h"
#include <cstdint>

namespace {

enum class sc_element_type : std::uint32_t {
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

struct sc_header {
	std::int32_t height;
	std::int32_t width;
	std::uint32_t number_of_data_sets;
	sc_element_type data_sets[10];
	float pose[16];
};

using sc_amplitude = float;
using sc_cartesian = float[3];
using sc_color = std::uint8_t[3];
using sc_normal = float[3];
using sc_polar = float[3];
using sc_texture = std::uint32_t[3];
using sc_validity = std::uint32_t;


}



namespace pcf {

std::size_t width() const {

}


std::size_t height() const {

}

void rewind() {

}

std::ptrdiff_t tell() {

}

void read(point_xyz*, std::size_t sz) {

}


void read(point_full*, std::size_t sz) {

}


}
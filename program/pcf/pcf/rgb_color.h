#ifndef PCF_RGB_COLOR_H_
#define PCF_RGB_COLOR_H_

#include <cstdint>

namespace pcf {

struct rgb_color {
	std::uint8_t r, g, b;
	
	rgb_color() = default;
	rgb_color(std::uint8_t nr, std::uint8_t ng, std::uint8_t nb) : r(nr), g(ng), b(nb) { }
	rgb_color(std::uint32_t rgb) : r(rgb >> 16), g(rgb >> 8), b(rgb) { }
	
	static const rgb_color black() { return 0x000000; }
};

}

#endif
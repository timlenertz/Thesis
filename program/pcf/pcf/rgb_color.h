#ifndef PCF_RGB_COLOR_H_
#define PCF_RGB_COLOR_H_

#include <cstdint>

namespace pcf {

/**
RGB color.
3 bytes composed of red, green, blue components.
*/
struct rgb_color {
	static const rgb_color black;
	static const rgb_color white;
	static const rgb_color red;
	static const rgb_color green;
	static const rgb_color blue;

	std::uint8_t r, g, b;
	
	rgb_color() = default;
	rgb_color(std::uint8_t nr, std::uint8_t ng, std::uint8_t nb) : r(nr), g(ng), b(nb) { }
	rgb_color(std::uint32_t rgb) : r(rgb >> 16), g(rgb >> 8), b(rgb) { }
};

}

#endif
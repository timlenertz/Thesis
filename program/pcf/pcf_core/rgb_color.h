#ifndef PCF_RGB_COLOR_H_
#define PCF_RGB_COLOR_H_

#include <cstdint>
#include <array>

namespace pcf {

/**
RGB color.
3 bytes composed of red, green, blue components. POD type intended for raw memory access.
*/
struct rgb_color {
	static const rgb_color black;
	static const rgb_color white;
	static const rgb_color red;
	static const rgb_color green;
	static const rgb_color blue;
	static const rgb_color yellow;
	static const rgb_color magenta;
	static const rgb_color cyan;

	std::uint8_t r, g, b;
	
	rgb_color() = default;
	rgb_color(std::uint8_t nr, std::uint8_t ng, std::uint8_t nb) : r(nr), g(ng), b(nb) { }
	rgb_color(std::uint32_t rgb) : r(rgb >> 16), g(rgb >> 8), b(rgb) { }
	
	/// Get color as 3 float values in [0, 1].
	std::array<float, 3> to_float() const;
};

}

#endif
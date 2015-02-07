#include "rgb_color.h"

namespace pcf {

const rgb_color rgb_color::black = 0x000000;
const rgb_color rgb_color::white = 0xffffff;
const rgb_color rgb_color::red = 0xff0000;
const rgb_color rgb_color::green = 0x00ff00;
const rgb_color rgb_color::blue = 0x0000ff;
const rgb_color rgb_color::yellow = 0xffff00;
const rgb_color rgb_color::magenta = 0xff00ff;
const rgb_color rgb_color::cyan = 0x00ffff;

std::array<float, 3> rgb_color::to_float() const {
	return std::array<float, 3>({
		float(r) / 255.f,
		float(g) / 255.f,
		float(b) / 255.f
	});
}


}
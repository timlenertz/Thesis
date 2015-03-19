#include "rgb_color.h"

namespace pcf {

namespace {
	template<typename T>
	std::uint8_t constrain_(T v) {
		if(v > T(255)) return 255;
		else if(v < T(0)) return 0;
		else return std::uint8_t(v);
	}	
}

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

rgb_color operator*(float c, const rgb_color& col) {
	return rgb_color(
		constrain_(c * (float)col.r),
		constrain_(c * (float)col.g), 
		constrain_(c * (float)col.b)
	);
}

rgb_color operator+(const rgb_color& col1, const rgb_color& col2) {
	return rgb_color(
		constrain_((int)col1.r + (int)col2.r),
		constrain_((int)col1.g + (int)col2.g), 
		constrain_((int)col1.b + (int)col2.b)
	);
}

rgb_color operator-(const rgb_color& col1, const rgb_color& col2) {
	return rgb_color(
		constrain_((int)col1.r - (int)col2.r),
		constrain_((int)col1.g - (int)col2.g), 
		constrain_((int)col1.b - (int)col2.b)
	);
}


}
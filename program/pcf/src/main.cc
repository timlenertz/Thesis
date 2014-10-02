#include "ply_reader.h"
#include <iostream>

using namespace pcf;

int main(int argc, const char* argv[]) {
	ply_reader reader(argv[1]);
	std::cout << "Length: " << reader.size() << std::endl;
	point_xyzrgb buf[100];
	reader.read(buf, 100);
	for(point_xyzrgb *p = buf, *buf_end = buf + 100; p != buf_end; ++p) {
		std::cout << "x=" << (*p)[0] << " ; " << "y=" << (*p)[1] << " ; " << "z=" << (*p)[2] << std::endl;
	}
}
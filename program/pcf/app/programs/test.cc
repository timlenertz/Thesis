#include "../program.h"
#include <iostream>

using namespace pcf;

PCF_PROGRAM(test) {
	void main() {
		std::cout << "test" << std::endl;
	}
};

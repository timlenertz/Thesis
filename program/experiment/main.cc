#include <cstdlib>
#include <iostream>

#include "relief.h"

#include <pcf/core.h>
#include <pcf/viewer.h>

using namespace pcf;

int main(int argc, const char* argv[]) {
	relief("output/relief.db");

	return EXIT_SUCCESS;
}
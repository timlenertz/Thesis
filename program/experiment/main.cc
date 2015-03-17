#include <cstdlib>
#include <iostream>

#include "hilo.h"
#include "projdown.h"

int main(int argc, const char* argv[]) {
	hilo("data/hi.ply", "output/hilo.db");
	
	return EXIT_SUCCESS;
}
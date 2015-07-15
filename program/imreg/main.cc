#include <iostream>
#include <Eigen/Eigen>
#include "imreg.h"

using namespace pcf;

int main(int argc, const char* argv[]) {
	std::cout << affine_image_registration(argv[1], argv[2]).matrix() << std::endl;
}
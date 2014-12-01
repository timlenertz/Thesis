#include "shell.h"
#include "program.h"
#include <iostream>

namespace pcf {

namespace {
	shell* global_shell_ = nullptr;
}

shell& shell::get() {
	if(! global_shell_) global_shell_ = new shell;
	return *global_shell_;
}


void shell::main() {
	std::cout << "shell" << std::endl;
}

}
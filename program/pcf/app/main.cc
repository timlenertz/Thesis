#include "glfw.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include "viewer_window.h"
#include "shell.h"

using namespace pcf;

static void glfw_error_(int code, const char* description) {
	std::cerr << "GLFW Error " << code << ": " << description << std::endl;
}

int main(int argc, const char* argv[]) {
	glfwSetErrorCallback(&glfw_error_);
	glfwInit();
	std::atexit(glfwTerminate);
	
	viewer_window viewer_win;

	std::thread shell_thread([&viewer_win]() {
		shell::set_viewer_window(viewer_win);
		shell::main();
	});

	viewer_win.run();	
	
	shell_thread.join();
	
	return EXIT_SUCCESS;
}
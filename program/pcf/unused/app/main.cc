#include "glfw.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <utility>
#include "viewer_window.h"
#include "shell.h"

using namespace pcf;

static void glfw_error_(int code, const char* description) {
	std::cerr << "GLFW Error " << code << ": " << description << std::endl;
}

int main(int argc, const char* argv[]) {
	// Setup GLFW.
	glfwSetErrorCallback(&glfw_error_);
	glfwInit();
	std::atexit(glfwTerminate);
	
	// Setup viewer window with OpenGL context.
	// Contains viewer and scene.
	viewer_window viewer_win;

	// Run shell on secondary thread.
	// Handles console IO. Signals viewer window to close when user exits.
	std::thread shell_thread(&shell::main, std::ref(viewer_win));
	
	// Open viewer window and run main thread.
	// Returns when user exited using shell.
	viewer_win.run();	
	
	// User has exited...
	
	// Wait until shell thread has finished.
	shell_thread.join();
	
	return EXIT_SUCCESS;
}
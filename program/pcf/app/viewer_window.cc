#include "viewer_window.h"

namespace pcf {

namespace {
	const char window_title_[] = "PCF Viewer";
	const int default_window_width_ = 800;
	const int default_window_height_ = 600;
}

viewer_window::viewer_window() {
	window_ = SDL_CreateWindow(
		"PCF Viewer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		default_window_width_,
		default_window_height_,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
	);
}

}
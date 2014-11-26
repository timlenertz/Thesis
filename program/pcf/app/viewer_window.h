#ifndef PCFAPP_VIEWER_WINDOW_H_
#define PCFAPP_VIEWER_WINDOW_H_

#include <SDL.h>
#include <>

namespace pcf {

class viewer_window {
private:
	SDL_Window* window_ = nullptr;

public:
	viewer_window();
	~viewer_window();
};

}

#endif
#ifndef PCFAPP_GLFW_H_
#define PCFAPP_GLFW_H_

#ifndef __APPLE__
	#ifdef _WIN32
		#include <windows.h>
	#endif
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#endif

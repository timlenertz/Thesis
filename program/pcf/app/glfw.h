#ifndef PCFAPP_GLFW_H_
#define PCFAPP_GLFW_H_

#ifdef __APPLE__
	#define GLFW_INCLUDE_GLCOREARB
#else
	#ifdef _WIN32
		#include <windows.h>
	#endif
	
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#endif

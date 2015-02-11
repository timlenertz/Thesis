#ifndef PCFUI_GL_H_
#define PCFUI_GL_H_

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#ifdef _WIN32
		#include <windows.h>
	#endif
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif

#endif
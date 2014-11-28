#ifndef PCFVW_GL_H_
#define PCFVW_GL_H_

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include <OpenGL/gl.h>
#else
	#ifdef _WIN32
		#include <windows.h>
	#endif
	#include <GL/gl.h>
#endif

#endif
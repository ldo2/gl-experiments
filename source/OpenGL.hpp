#ifndef OPEN_GL_HEADERS_HPP__
#define OPEN_GL_HEADERS_HPP__

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

void openGlLibraryInitialize(int &argc, char *argv[]);

#endif // OPEN_GL_HEADERS_HPP__
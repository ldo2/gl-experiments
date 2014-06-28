#include "GlutApplication.hpp"

#include <iostream>
#include <stdexcept>

GlutApplication *GlutApplication::instance = nullptr;

class GlutApplicationDispatcher {
public:
	GlutApplicationDispatcher() = delete;

	static void display() {
		GlutApplication::instance->onDisplay();
	}
	
	static void reshape(int width, int height) {
		GlutApplication::instance->updateSize(width, height);
		GlutApplication::instance->onReshape(width, height);
	}
	
	static void keyboard(unsigned char key, int x, int y) {
		GlutApplication::instance->onKeyboard(key, x, y);
	}
	
	static void mouse(int button, int state, int x, int y) {
		GlutApplication::instance->onMouse(button, state, x, y);
	}
	
	static void motion(int x, int y) {
		GlutApplication::instance->onMotion(x, y);
	}
	
	static void timer(int value) {
		GlutApplication::instance->onTimer(value);
	}
};

GlutApplication::GlutApplication(int &argc, char *argv[], 
		const std::string& name, int width, int height) : 
	theName(name), theWidth(width), theHeight(height) 
{
	if (GlutApplication::instance) {
		throw std::runtime_error("double Application initialization");
	}
	GlutApplication::instance = this;
		
	openGlLibraryInitialize(argc, argv);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL | GLUT_DOUBLE);
		
	glutInitWindowSize(width, height);
	glutCreateWindow(name.c_str());
	
	glutDisplayFunc(&GlutApplicationDispatcher::display);
	glutReshapeFunc(&GlutApplicationDispatcher::reshape);
	glutKeyboardFunc(&GlutApplicationDispatcher::keyboard);
	glutMouseFunc(&GlutApplicationDispatcher::mouse);
	glutMotionFunc(&GlutApplicationDispatcher::motion);
}
	
void GlutApplication::run() {
	glutMainLoop();
}
	
void GlutApplication::timer(unsigned int msecs, int value) {
	glutTimerFunc(msecs, &GlutApplicationDispatcher::timer, value);
}

void GlutApplication::updateSize(int width, int height) {
	theWidth  = width;
	theHeight = height;
}

void GlutApplication::onDisplay() {
}

void GlutApplication::onReshape(int width, int height) {
}

void GlutApplication::onKeyboard(unsigned char key, int x, int y) {
}

void GlutApplication::onMouse(int button, int state, int x, int y) {
}

void GlutApplication::onMotion(int x, int y) {
}

void GlutApplication::onTimer(int value) {
}

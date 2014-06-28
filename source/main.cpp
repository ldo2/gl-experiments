#include <iostream>
#include <cmath>

#include "GlutApplication.hpp"
#include "TrivialGlCamera.hpp"

class MyApplication : public GlutApplication {
public:
	MyApplication(int argc, char *argv[]) : 
		GlutApplication(argc, argv, "OpenGL application"), 
		camera(1.0), polygoneMode(GL_FILL)
	{	
		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0);
		
		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHTING);
		// glEnable(GL_STENCIL);
		
		glViewport(0, 0, width(), height());
		
		camera.setZoom(5.0);
	}
	
	virtual ~MyApplication() {
	}

	void help() {
		std::cout << 
    		"Control:\n"
    		"\tMouse          rotate camera\n"   
    		"\tKeys '+'/'-'   camera zoom\n"
    		"\tKey  'm'       change polygone mode\n";
	}
	
	void togglePolygoneMode() {
		if (polygoneMode == GL_FILL) {
			polygoneMode = GL_LINE;
		} else {
			polygoneMode = GL_FILL;
		}
	}
	
protected:
	void onDisplay() {
		glClearColor(0.33, 0.33, 0.58, 1.0);
		// glClearStencil(0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		camera.setGlMatricies(aspectRatio());

		setLight();

		glPolygonMode(GL_FRONT_AND_BACK, polygoneMode);
		glutSolidCube(1.0);

		glFlush();
		glutSwapBuffers();
	}

	void onKeyboard(unsigned char key, int x, int y) {
		switch (key) {
		case '+':
			camera.zoomIn();
			break;
		case '-':
			camera.zoomOut();
			break;
		case 'm':
			togglePolygoneMode();
			break;
		}

		glutPostRedisplay();
	}

	void onMouse(int button, int state, int x, int y) {
		if (button == GLUT_LEFT_BUTTON) {
			camera.onMouse(x, y, state == GLUT_DOWN);
		}
	}

	void onMotion(int x, int y) {
		camera.onMouseMove(x, y);
	}

	void onReshape(int width, int height) {
		glViewport(0, 0, width, height);
		glutPostRedisplay();
	}

private:	
	TrivialGlCamera camera;
	GLenum polygoneMode;
	
	GLdouble aspectRatio() const {
		return GLdouble(width())/GLdouble(height());
	}

	void setLight() {
		static GLfloat lightDiffuse[]  = {1.0, 1.0, 1.0, 1.0};
		static GLfloat lightPosition[] = {1.0, 1.0, 3.0, 0.0};
	
		glLightfv(GL_LIGHT0, GL_DIFFUSE , lightDiffuse );
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
};

int main(int argc, char *argv[]) {
	try {
		MyApplication application(argc, argv);
		application.help();
		application.run();
	} catch(std::exception& error) {
		std::cerr << "CATCH EXCEPTION: " << error.what() << std::endl;
	}
	return 0;
}

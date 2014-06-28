#ifndef TRIVIAL_GL_CAMERA_HPP__
#define TRIVIAL_GL_CAMERA_HPP__

#include "OpenGL.hpp"

class TrivialGlCamera {
public:
	static constexpr GLdouble DISTANCE_GAP = 0.1;

	TrivialGlCamera(GLdouble radius) : 
		rotationX(20.0), rotationY(30.0), rotationZ(0.0), 
		sceneRadius(radius), minDistance(radius + DISTANCE_GAP), distance(minDistance), 
		zoomSensitivity(0.1), mouseSensitivity(0.75) 
	{
	}
	
	void setGlMatricies(GLdouble aspect) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, aspect, distance - sceneRadius, distance + sceneRadius);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslated(0.0, 0.0, -distance);

		glRotated(rotationX, 1.0, 0.0, 0.0);
		glRotated(rotationY, 0.0, 1.0, 0.0);
		glRotated(rotationZ, 0.0, 0.0, 1.0);
	}
	
	void setSceneRadius(GLdouble radius) {
		sceneRadius = radius;
		minDistance = radius + DISTANCE_GAP;
		if (distance > minDistance) {
			distance = minDistance;
		}
	}

	void onMouse(int x, int y, bool click) {
		mouseX = x;
		mouseY = y;
		mouseRotationActive = click;
	}
	
	void onMouseMove(int x, int y) {
		if (mouseRotationActive) {
			rotationX += mouseSensitivity*(y - mouseY); 
			rotationY += mouseSensitivity*(x - mouseX),
			
			mouseX = x; 
			mouseY = y;
			
			glutPostRedisplay();
		}
	}
	
	void setZoom(GLdouble newDistance) {
		if (newDistance >= minDistance) {
			distance = newDistance;
		}
	}
	
	void zoomIn() {
		setZoom(distance * (1.0 - zoomSensitivity));	
	}
	
	void zoomOut() {
		setZoom(distance * (1.0 + zoomSensitivity));
	}
	
private:
	// camera attributes
	GLdouble rotationX;
	GLdouble rotationY;
	GLdouble rotationZ;
	
	GLdouble sceneRadius;
	GLdouble minDistance;
	GLdouble distance;

	// camera control attributes
	GLdouble zoomSensitivity;
	GLdouble mouseSensitivity;

	bool mouseRotationActive;
	int mouseX, mouseY;	
};

#endif // TRIVIAL_GL_CAMERA_HPP__

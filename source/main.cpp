#include <iostream>
#include <random>

#include <cmath>

#include "GlutApplication.hpp"
#include "TrivialGlCamera.hpp"
#include "GlModel.hpp"
#include "GlMeshBuilder.hpp"

class CubeBuilder : public GlMeshBuilder {
public:
	static constexpr int SIDES = 6;
	
	CubeBuilder(int width, int height) :  
		GlMeshBuilder((SIDES + 1)*width, (SIDES + 1)*height),
		theSideWidth(width), theSideHeight(height)
	{
	}
	
	virtual ~CubeBuilder() {
	}
	
protected:
	virtual void generateModel() {
		int sideSize = theSideWidth * theSideHeight;
		int meshSize = (theSideWidth - 1) * (theSideHeight - 1) * 6;

		for (int side = 0; side < SIDES; ++side) {
			setFace(side);
			generateIndices(theSideWidth, theSideHeight, 
					side * meshSize, side * sideSize);
			generateVerticesAttributes(theSideWidth, theSideHeight, 
					side * sideSize);
		}
	}
	
	virtual void evalVertex(GLfloat (&vertex)[4], GLfloat x, GLfloat y) {
		vertex[0] = sideBase[0] + sideVectorU[0] * x + sideVectorV[0] * y;
		vertex[1] = sideBase[1] + sideVectorU[1] * x + sideVectorV[1] * y;
		vertex[2] = sideBase[2] + sideVectorU[2] * x + sideVectorV[2] * y;
		vertex[3] = 1.0;
	}
	
	virtual void evalNormal(GLfloat (&normal)[3], GLfloat x, GLfloat y) {
		normal[0] = sideNormal[0];
		normal[1] = sideNormal[1];
		normal[2] = sideNormal[2];
	}
	
	virtual void evalTexCoord(GLfloat (&texcoord)[4], GLfloat x, GLfloat y) {
		texcoord[0] = x;
		texcoord[1] = y;
		texcoord[2] = sideCoord;
		texcoord[3] = 0.0;
	}

private:
	static const GLfloat cubeFaces[6][5][3];

	int theSideWidth;
	int theSideHeight;
	
	GLfloat sideBase[3];
	GLfloat sideVectorU[3];
	GLfloat sideVectorV[3];
	GLfloat sideNormal[3];
	GLfloat sideCoord;
	
	void setVec3(GLfloat (&left)[3], const GLfloat (&right)[3]) {
		left[0] = right[0];
		left[1] = right[1];
		left[2] = right[2];
	}
	
	void setFace(int side) {
		setVec3(sideBase, cubeFaces[side][0]);
		setVec3(sideVectorU, cubeFaces[side][1]);
		setVec3(sideVectorV, cubeFaces[side][2]);
		setVec3(sideNormal, cubeFaces[side][3]);
		sideCoord = cubeFaces[side][4][0];
	}
};

const GLfloat CubeBuilder::cubeFaces[6][5][3] = {
	{{ 0.5, -0.5, -0.5}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, { 1.0,  0.0,  0.0}, {0.0, 0.0, 0.0}},
	{{-0.5, -0.5, -0.5}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {-1.0,  0.0,  0.0}, {1.0, 0.0, 0.0}},
	{{-0.5,  0.5, -0.5}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, { 0.0,  1.0,  0.0}, {2.0, 0.0, 0.0}},
	{{-0.5, -0.5, -0.5}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, { 0.0, -1.0,  0.0}, {3.0, 0.0, 0.0}},
	{{-0.5, -0.5,  0.5}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, { 0.0,  0.0,  1.0}, {4.0, 0.0, 0.0}},
	{{-0.5, -0.5, -0.5}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, { 0.0,  0.0, -1.0}, {5.0, 0.0, 0.0}}
};

class SphereBuilder : public CubeBuilder {
public:
	SphereBuilder(int width, int height) : 
		CubeBuilder(width, height) 
	{
	}
	
protected:
	virtual void evalVertex(GLfloat (&vertex)[4], GLfloat x, GLfloat y) {
		// uniform distribution for points
		x = 0.5*tan((0.5*x - 0.25)*M_PI) + 0.5;
		y = 0.5*tan((0.5*y - 0.25)*M_PI) + 0.5;
	
		CubeBuilder::evalVertex(vertex, x, y);
		GLfloat length = sqrt(vertex[0] * vertex[0] + 
				vertex[1] * vertex[1] + vertex[2] * vertex[2]);
		vertex[0] /= length;
		vertex[1] /= length;
		vertex[2] /= length;
	}
	
	virtual void evalNormal(GLfloat (&normal)[3], GLfloat x, GLfloat y) {
		GLfloat vertex[4];
		evalVertex(vertex, x, y);
		
		normal[0] = vertex[0];
		normal[1] = vertex[1];
		normal[2] = vertex[2];
	}
};

class DistortedSphereBuilder : public CubeBuilder {
public:
	DistortedSphereBuilder(int width, int height) : 
		CubeBuilder(width, height), theDistortionMap(nullptr), 
		theMapWidth(width + 2), theMapHeight(height + 2)
	{
		theDistortionMap = new GLfloat [theMapWidth * theMapHeight];
		
		std::default_random_engine generator;
		std::uniform_real_distribution<GLfloat> distribution(0.6, 1.0);
		
		for (int i =0; i < theMapWidth; ++i) {
			for (int j = 0; j < theMapHeight; ++j) {
				theDistortionMap[i*theMapHeight + j] = distribution(generator);
			}
		}
		
		for (int s = 0; s < 16; ++s) {
		for (int i = 1; i < theMapWidth - 1; ++i) {
			for (int j = 1; j < theMapHeight - 1; ++j) {
				theDistortionMap[i*theMapHeight + j] = 0.2 * (
					theDistortionMap[i*theMapHeight + j] + 
					theDistortionMap[i*theMapHeight + j + 1] + 
					theDistortionMap[i*theMapHeight + j - 1] + 
					theDistortionMap[(i + 1)*theMapHeight + j] + 
					theDistortionMap[(i - 1)*theMapHeight + j]
				);
			}
		}
		}
	}
	
	virtual ~DistortedSphereBuilder() {
		delete[] theDistortionMap;
	}
	
	DistortedSphereBuilder(const DistortedSphereBuilder&) = delete;
	DistortedSphereBuilder& operator =(const DistortedSphereBuilder&) = delete;
	
protected:
	virtual void evalVertex(GLfloat (&vertex)[4], GLfloat x, GLfloat y) {
		int i(x*(theMapWidth - 3) + 1), j(y*(theMapHeight - 3) + 1);
		GLfloat radius = theDistortionMap[i*theMapHeight + j];
	
		// uniform distribution for points
		x = 0.5*tan((0.5*x - 0.25)*M_PI) + 0.5;
		y = 0.5*tan((0.5*y - 0.25)*M_PI) + 0.5;
	
		CubeBuilder::evalVertex(vertex, x, y);
		GLfloat length = sqrt(vertex[0] * vertex[0] + 
				vertex[1] * vertex[1] + vertex[2] * vertex[2]);
		GLfloat factor = radius/length;
		vertex[0] *= factor;
		vertex[1] *= factor;
		vertex[2] *= factor;
	}
	
	virtual void evalNormal(GLfloat (&normal)[3], GLfloat x, GLfloat y) {
		GLfloat vertex[4][4];
		GLfloat dx = 1.0/(theMapWidth - 3), dy = 1.0/(theMapHeight - 3);
		
		evalVertex(vertex[0], x - dx, y);
		evalVertex(vertex[1], x + dx, y);
		evalVertex(vertex[2], x, y - dy);
		evalVertex(vertex[3], x, y + dy);
		
		vertex[1][0] -= vertex[0][0];
		vertex[1][1] -= vertex[0][1];
		vertex[1][2] -= vertex[0][2];
		
		vertex[3][0] -= vertex[2][0];
		vertex[3][1] -= vertex[2][1];
		vertex[3][2] -= vertex[2][2];
		
		normal[0] = vertex[1][1]*vertex[3][2] - vertex[1][2]*vertex[3][1];
		normal[1] = - (vertex[1][0]*vertex[3][2] - vertex[1][2]*vertex[3][0]);
		normal[2] = vertex[1][0]*vertex[3][1] - vertex[1][1]*vertex[3][0];
	}
	
private:
	GLfloat *theDistortionMap;
	int theMapWidth;
	int theMapHeight;
};

class MyApplication : public GlutApplication {
public:
	MyApplication(int argc, char *argv[]) : 
		GlutApplication(argc, argv, "OpenGL application"), 
		camera(1.0), polygoneMode(GL_FILL), 
		model(GlModel::ALL_BUFFERS_BITS)
	{	
		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0);
		
		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHTING);
		// glEnable(GL_STENCIL);
		
		glViewport(0, 0, width(), height());
		
		camera.setZoom(5.0);
		
		DistortedSphereBuilder builder(64, 64);
		builder.build(model, GlModel::ALL_BUFFERS_BITS);
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
		model.draw();

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
	
	GlModel model;
	
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

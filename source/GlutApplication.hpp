#ifndef GLUT_APPLICATION_HPP__
#define GLUT_APPLICATION_HPP__

#include <string>
#include "OpenGL.hpp"

class GlutApplicationDispatcher;

class GlutApplication {
private:
	static GlutApplication *instance;
	
	std::string theName;
	int theWidth;
	int theHeight;	 
	
	void updateSize(int width, int height);
	
public:
	static constexpr int DEFAULT_WIDTH  = 640;
	static constexpr int DEFAULT_HEIGHT = 480;
	
	GlutApplication(int &argc, char *argv[], const std::string& name, 
			int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
		
	virtual ~GlutApplication() {
	}
	
	const std::string& name() const {
		return theName;
	}
	
	int width() const {
		return theWidth;
	}
	
	int height() const {
		return theHeight;
	}
	
	void run();

	void timer(unsigned int msecs, int value);

protected:
	virtual void onDisplay();
	virtual void onReshape(int width, int height);
	virtual void onKeyboard(unsigned char key, int x, int y);
	virtual void onMouse(int button, int state, int x, int y);
	virtual void onMotion(int x, int y);
	virtual void onTimer(int value);

friend class GlutApplicationDispatcher;
};


#endif // GLUT_APPLICATION_HPP__

#ifndef GL_MODEL_HPP__
#define GL_MODEL_HPP__

#include "OpenGL.hpp"

class GlModel {
public:
	enum BufferIndex {
		INDEX_BUFFER,
		VERTEX_BUFFER,
		NORMAL_BUFFER,
		TEXCOORD_BUFFER,
		N_BUFFERS
	};
	
	enum BufferBit {
		INDEX_BUFFER_BIT 	= 1,
		VERTEX_BUFFER_BIT 	= 2,
		NORMAL_BUFFER_BIT 	= 4,
		TEXCOORD_BUFFER_BIT = 8,
		ALL_BUFFERS_BITS = 15
	};
	
	GlModel(int mask = 0);
	
	~GlModel();
	
	GlModel(const GlModel&) = delete;
	GlModel& operator =(const GlModel&) = delete;
	
	void setMask(int mask) {
		theMask = mask;
	}
	
	void setSize(unsigned size) {
		theSize = size;
	}
	
	int mask() const {
		return theMask;
	}
	
	GLuint indices() const {
		return theBuffers[INDEX_BUFFER];
	}
	
	GLuint vertices() const {
		return theBuffers[VERTEX_BUFFER];
	}
	
	GLuint normals() const {
		return theBuffers[NORMAL_BUFFER];
	}
	
	GLuint textcoords() const {
		return theBuffers[TEXCOORD_BUFFER];
	}

	unsigned size() const {
		return theSize;
	}

	void draw() const;
	
private:
	GLuint theBuffers[N_BUFFERS];
	int theMask;
	unsigned theSize;
};

#endif // GL_MODEL_HPP__
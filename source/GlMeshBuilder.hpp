#ifndef GL_MESH_BUILDER_HPP__
#define GL_MESH_BUILDER_HPP__

#include "OpenGL.hpp"
#include "GlModel.hpp"
#include "GlArray.hpp"

class GlMeshBuilder {
public:
	GlMeshBuilder(int width, int height);

	virtual ~GlMeshBuilder();
	
	virtual void build(GlModel& model, int mask);
	
protected:
	virtual void generateModel();

	void generateIndices(int width, int height, 
			int offset = 0, int indexOffset = 0);
	
	void generateVerticesAttributes(int width, int height, int offset = 0);
	
	virtual void evalVertex(GLfloat (&vertex)[4], GLfloat x, GLfloat y);
	
	virtual void evalNormal(GLfloat (&normal)[3], GLfloat x, GLfloat y);
	
	virtual void evalTexCoord(GLfloat (&texcoord)[4], GLfloat x, GLfloat y);
	
	void buildModel(GlModel& model, int mask);
	
	void buildIndices(GlModel& model);
	
	void buildVertices(GlModel& model);
	
	void buildNormals(GlModel& model);
	
	void buildTexCoords(GlModel& model);
	
private:
	int theWidth;
	int theHeight;
	
	GlArray<GLuint> indices;
	GlArray<GLfloat, 4> vertices;
	GlArray<GLfloat, 3> normals;
	GlArray<GLfloat, 4> texcoords;
};

#endif // GL_MESH_BUILDER_HPP__

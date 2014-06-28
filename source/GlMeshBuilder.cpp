#include "GlMeshBuilder.hpp"

#include <iostream>

GlMeshBuilder::GlMeshBuilder(int width, int height) : 
	theWidth(width), theHeight(height),
	indices((width - 1) * (height - 1) * 6),
	vertices(width * height), 
	normals(width * height), 
	texcoords(width * height) 
{
}

GlMeshBuilder::~GlMeshBuilder() {
}
	
void GlMeshBuilder::build(GlModel& model, int mask) {
	generateModel();
	buildModel(model, mask);
}

void GlMeshBuilder::generateModel() {
	generateIndices(theWidth, theHeight);
	generateVerticesAttributes(theWidth, theHeight);
}

void GlMeshBuilder::buildModel(GlModel& model, int mask) {
	if (mask & GlModel::INDEX_BUFFER_BIT) {
		buildIndices(model);
	}
	if (mask & GlModel::VERTEX_BUFFER_BIT) {
		buildVertices(model);
	}
	if (mask & GlModel::NORMAL_BUFFER_BIT) {
		buildNormals(model);
	}
	if (mask & GlModel::TEXCOORD_BUFFER_BIT) {
		buildTexCoords(model);
	}
	model.setMask(mask);
}
	
void GlMeshBuilder::generateIndices(int width, int height, int offset, int indexOffset) {
    int i, j, k = offset;

	for (i = 0; i < width - 1; ++i) {
		for (j = 0; j < height - 1; ++j) {
			indices.at(k++)[0] = indexOffset + (i * height + j);
			indices.at(k++)[0] = indexOffset + (i * height + j + 1);
			indices.at(k++)[0] = indexOffset + ((i + 1) * height + j);
				
			indices.at(k++)[0] = indexOffset + (i * height + j + 1);
			indices.at(k++)[0] = indexOffset + ((i + 1) * height + j);
			indices.at(k++)[0] = indexOffset + ((i + 1) * height + j + 1);
		}
	}
}
	
void GlMeshBuilder::generateVerticesAttributes(int width, int height, int offset) {
	int i, j, k = offset;
	GLfloat x, y;
        
	for (i = 0; i < width; ++i) {
		for (j = 0; j < height; ++j) {
			x = GLfloat(j)/(height - 1);
			y = GLfloat(i)/(width - 1);

			evalVertex(vertices.at(k), x, y);
			evalNormal(normals.at(k), x, y);
			evalTexCoord(texcoords.at(k), x, y);
			
			++k;
		} 
	}
}

void GlMeshBuilder::evalVertex(GLfloat (&vertex)[4], GLfloat x, GLfloat y) {
	vertex[0] = x - 0.5;
	vertex[1] = y - 0.5;
	vertex[2] = 0.0;
	vertex[3] = 1.0;
}

void GlMeshBuilder::evalNormal(GLfloat (&normal)[3], GLfloat x, GLfloat y) {
	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 1.0;
}
	
void GlMeshBuilder::evalTexCoord(GLfloat (&texcoord)[4], GLfloat x, GLfloat y) {
	texcoord[0] = x;
	texcoord[1] = y;
	texcoord[2] = 0.0;
	texcoord[3] = 0.0;
}
	
void GlMeshBuilder::buildIndices(GlModel& model) {
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, model.indices());
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, indices.byteSize(), 
			indices.data(), GL_STREAM_DRAW_ARB);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

	model.setSize(indices.size());
}
	
void GlMeshBuilder::buildVertices(GlModel& model) {
	glBindBufferARB(GL_ARRAY_BUFFER, model.vertices());
	glBufferDataARB(GL_ARRAY_BUFFER, vertices.byteSize(), 
			vertices.data(), GL_STREAM_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
}
	
void GlMeshBuilder::buildNormals(GlModel& model) {
	glBindBufferARB(GL_ARRAY_BUFFER, model.normals());
	glBufferDataARB(GL_ARRAY_BUFFER, normals.byteSize(), 
			normals.data(), GL_STREAM_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
}
	
void GlMeshBuilder::buildTexCoords(GlModel& model) {	
	// TODO
}

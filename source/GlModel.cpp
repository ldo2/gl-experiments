#include "GlModel.hpp"

GlModel::GlModel(int mask) : theMask(mask), theSize(0) {
	glGenBuffersARB(N_BUFFERS, theBuffers);
}
	
GlModel::~GlModel() {
	glDeleteBuffersARB(N_BUFFERS, theBuffers);		
}
	
void GlModel::draw() const {
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER, vertices());
	glVertexPointer(4, GL_FLOAT, 0, nullptr);

	if (theMask & NORMAL_BUFFER_BIT) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER, normals());
		glNormalPointer(GL_FLOAT, 0, nullptr);
	}
		
	if (theMask & TEXCOORD_BUFFER_BIT) {
		// TODO
	}

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, indices());
	glIndexPointer(GL_UNSIGNED_INT, sizeof(GLuint), 0);

	glDrawElements(GL_TRIANGLES, theSize, GL_UNSIGNED_INT, 0);

	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPopClientAttrib();
}

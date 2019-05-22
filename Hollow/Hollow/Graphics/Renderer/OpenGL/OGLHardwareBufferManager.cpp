#include "OGLHardwareBufferManager.h"

VertexBuffer* OGLHardwareBufferManager::createVertexBuffer(Vertex* data, unsigned int numVertices)
{
	OGLVertexBuffer* buffer = new OGLVertexBuffer(sizeof(Vertex), numVertices);

	glGenVertexArrays(1, &buffer->VAO);
	glBindVertexArray(buffer->VAO);

	glGenBuffers(1, &buffer->VBO);

	glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), data, GL_STATIC_DRAW);

	glVertexAttribPointer(POSITION_LOCATION,	3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glVertexAttribPointer(TEX_COORD_LOCATION,	2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glVertexAttribPointer(NORMAL_LOCATION,		3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glVertexAttribPointer(TANGENT_LOCATION,		3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glVertexAttribPointer(BITANGENT_LOCATION,	3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glVertexAttribIPointer(BONE_ID_LOCATION,	4, GL_INT,			   4 * sizeof(unsigned int), 0);
	glVertexAttribPointer(WEIGHTS_LOCATION,		4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return buffer;
}

IndexBuffer* OGLHardwareBufferManager::createIndexBuffer(unsigned int* data, unsigned int numIndices)
{
	OGLIndexBuffer* buffer = new OGLIndexBuffer(sizeof(unsigned int), numIndices);

	glGenBuffers(1, &buffer->VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->VBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int), data, GL_STATIC_DRAW);

	return buffer;
}

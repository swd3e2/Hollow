#pragma once

#ifndef HW_OGL_HARDWARE_BUFFER_MANAGER_H
#define HW_OGL_HARDWARE_BUFFER_MANAGER_H

#include "Hollow/Graphics/HardwareBufferManager.h"
#include "OGLPrerequisites.h"
#include "OGLVertexBuffer.h"
#include "OGLIndexBuffer.h"

#define POSITION_LOCATION		0
#define TEX_COORD_LOCATION		1
#define NORMAL_LOCATION			2
#define TANGENT_LOCATION		3
#define BITANGENT_LOCATION		4
#define BONE_ID_LOCATION		5
#define WEIGHTS_LOCATION		6

class OGLHardwareBufferManager : public HardwareBufferManager
{
public:
	virtual VertexBuffer* createVertexBuffer(Vertex* data, unsigned int numVertices) override;
	virtual IndexBuffer* createIndexBuffer(unsigned int* data, unsigned int numIndices) override;
};

#endif
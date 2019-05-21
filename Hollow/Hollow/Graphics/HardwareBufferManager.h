#pragma once

#ifndef HW_I_HARDWARE_BUFFER_MANAGER_H
#define HW_I_HARDWARE_BUFFER_MANAGER_H

#include "Renderer/Base/VertexBuffer.h"
#include "Renderer/Base/IndexBuffer.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Graphics/Vertex.h"

class HardwareBufferManager : public CModule<HardwareBufferManager>
{
public:
	HardwareBufferManager();
	~HardwareBufferManager();
	virtual VertexBuffer* createVertexBuffer(Vertex* data, unsigned int numVertices) = 0;
	virtual IndexBuffer* createIndexBuffer(unsigned int* data, unsigned int numIndices) = 0;
};


#endif
#pragma once

#ifndef HW_I_HARDWARE_BUFFER_MANAGER_H
#define HW_I_HARDWARE_BUFFER_MANAGER_H

#include "Renderer/Base/VertexBuffer.h"
#include "Renderer/Base/IndexBuffer.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Graphics/Vertex.h"

class HOLLOW_API HardwareBufferManager : public CModule<HardwareBufferManager>
{
public:
	HardwareBufferManager();
	~HardwareBufferManager();
	virtual VertexBuffer* createVertexBuffer(Vertex* data, int numVertices) = 0;
	virtual IndexBuffer* createIndexBuffer(unsigned int* data, int numIndices) = 0;
};


#endif
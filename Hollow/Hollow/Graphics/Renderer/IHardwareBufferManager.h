#pragma once

#ifndef HW_I_HARDWARE_BUFFER_MANAGER_H
#define HW_I_HARDWARE_BUFFER_MANAGER_H

#include "Base/IVertexBuffer.h"
#include "Base/IIndexBuffer.h"
#include "Hollow/Core/CModule.h"

class IHardwareBufferManager : public CModule<IHardwareBufferManager>
{
public:
	virtual IVertexBuffer* createVertexBuffer() = 0;
	virtual IIndexBuffer* createIndexBuffer() = 0;
};


#endif
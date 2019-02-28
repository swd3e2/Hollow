#pragma once
#include "IBuffer.h"

class IRenderable
{
protected:
	IBuffer* vertexbuffer;

public:
	virtual void SetBuffer(IBuffer* buffer) = 0;
	virtual IBuffer* GetBuffer() = 0;
};
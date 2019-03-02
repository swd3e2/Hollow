#pragma once
#include "IBuffer.h"
#include "Hollow/Platform.h"

class HOLLOW_API IRenderable
{
protected:
	IBuffer* vertexbuffer;
public:
	/*virtual void SetBuffer(IBuffer* buffer) = 0;
	virtual IBuffer* GetBuffer() = 0;*/
};
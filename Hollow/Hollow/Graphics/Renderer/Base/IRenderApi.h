#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "IVertexBuffer.h"

class HOLLOW_API IRenderApi : public CModule<IRenderApi>
{
protected:
public:
	virtual void SetVertexBuffer(const IVertexBuffer& vertexBuffer) = 0;
};
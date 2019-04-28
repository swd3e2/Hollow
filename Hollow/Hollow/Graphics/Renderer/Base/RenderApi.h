#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "IndexBuffer.h"
#include "Shader.h"

class HOLLOW_API RenderApi : public CModule<RenderApi>
{
protected:
public:
	virtual void SetIndexBuffer(IndexBuffer*) = 0;
	virtual void SetVertexBuffer(VertexBuffer*) = 0;
	virtual void SetTexture(UINT, Texture*) = 0;
	virtual void SetTexture(UINT, RenderTarget*) = 0;
	virtual void SetShader(Shader*) = 0;
};
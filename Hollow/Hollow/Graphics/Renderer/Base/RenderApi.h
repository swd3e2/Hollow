#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ShaderProgram.h"

class HOLLOW_API RenderApi : public CModule<RenderApi>
{
protected:
public:
	virtual void startUp() = 0;
	virtual void SetIndexBuffer(IndexBuffer*) = 0;
	virtual void SetVertexBuffer(VertexBuffer*) = 0;
	virtual void SetTexture(UINT, Texture*) = 0;
	virtual void SetTexture(UINT, RenderTarget*) = 0;
	virtual void SetShader(ShaderProgram*) = 0;
};
#pragma once

#include "Hollow/Graphics/Renderer/Base/RenderTarget.h"

class OGLRenderTarget : public RenderTarget
{
public:
	unsigned int FBO;
	unsigned int texture;
	unsigned int depth;
	unsigned int renderBuffer;
public:
	OGLRenderTarget(int width, int height) : 
	RenderTarget(width, height), FBO(0)
	{}
};
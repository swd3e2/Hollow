#pragma once

#include "Hollow/Graphics/Renderer/Base/RenderTarget.h"

class OGLRenderTarget : public RenderTarget
{
public:
	unsigned int FBO;
public:
	OGLRenderTarget(int width, int height) : 
	RenderTarget(width, height), FBO(0)
	{}
};
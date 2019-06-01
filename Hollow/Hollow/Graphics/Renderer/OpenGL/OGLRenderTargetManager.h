#pragma once

#include "Hollow/Graphics/RenderTargetManager.h"
#include "OGLPrerequisites.h"
#include "OGLRenderTarget.h"
#include "Hollow/Common/Log.h"

class OGLRenderTargetManager : public RenderTargetManager
{
public:
	virtual RenderTarget* create(int width, int height);
};
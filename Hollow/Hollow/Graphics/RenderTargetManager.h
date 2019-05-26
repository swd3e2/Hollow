#pragma once

#include "Renderer/Base/RenderTarget.h"
#include "Hollow/Core/CModule.h"

class RenderTargetManager : public CModule<RenderTargetManager>
{
public:
	RenderTargetManager() { setStartedUp(); }
	~RenderTargetManager() { setShutdown(); }
	virtual RenderTarget* create(int width, int height) = 0;
};
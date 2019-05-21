#pragma once


#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "Graphics/Renderer/Base/RenderApi.h"

#ifdef D3D11
#include "Renderer/DirectX/D3D11RenderApi.h"
#endif
#ifdef OPENGL
#include "Renderer/OpenGL/OGLRenderApi.h"
#endif

class RenderApiManager : public CModule<RenderApiManager>
{
public:
	RenderApiManager()
	{
		setStartedUp();
	}

	~RenderApiManager()
	{
		setShutdown();
	}

	RenderApi* initialize(int width, int height)
	{
		RenderApi* renderer = nullptr;
#ifdef D3D11
		renderer = new D3D11RenderApi(width, height);
#endif
#ifdef OPENGL
		renderer = new OGLRenderApi(width, height);
#endif
		return renderer;
	}
};
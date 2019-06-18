#pragma once

#ifndef HW_RENDER_API_MANAGER_H
#define HW_RENDER_API_MANAGER_H

#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "Renderer/Base/RenderApi.h"
#include "Renderer/DirectX/D3D11RenderApi.h"
#include "Renderer/OpenGL/OGLRenderApi.h"
#include "RendererType.h"

namespace Hollow {
	class RenderApiManager : public CModule<RenderApiManager>
	{
	private:
		RendererType rendererType;
		RenderApi* renderer;
	public:
		RenderApiManager(RendererType type) :
			rendererType(type)
		{ 
			setStartedUp(); 
		}

		~RenderApiManager() 
		{ 
			setShutdown(); 
		}

		RenderApi* initialize(int width, int height)
		{
			switch (rendererType)
			{
				case RendererType::None: return nullptr;
				case RendererType::OpenGL: return (renderer = new OGLRenderApi(width, height));
				case RendererType::DirectX: return (renderer = new D3D11RenderApi(width, height));
			}
		}

		inline RendererType getRendererType() const { return rendererType; }
	};
}

#endif
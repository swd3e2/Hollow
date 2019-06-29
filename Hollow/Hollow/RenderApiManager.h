#pragma once

#ifndef HW_RENDER_API_MANAGER_H
#define HW_RENDER_API_MANAGER_H

#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "Graphics/Renderer/Base/RenderApi.h"
#include "Graphics/Renderer/DirectX/D3D11RenderApi.h"
#include "Graphics/Renderer/OpenGL/OGLRenderApi.h"
#include "Graphics/RendererType.h"

namespace Hollow {
	class RenderApiManager
	{
	private:
		RendererType rendererType;
		RenderApi* renderer;
	public:
		RenderApiManager(RendererType type) : rendererType(type) {}

		RenderApi* Initialize(int width, int height)
		{
			switch (rendererType)
			{
				case RendererType::None: return nullptr;
				case RendererType::OpenGL: {
					RenderApi::startUp<OGLRenderApi>(width, height);
				} break;
				case RendererType::DirectX: {
					RenderApi::startUp<D3D11RenderApi>(width, height);
				} break;
			}

			return RenderApi::instance();
		}

		inline RendererType getRendererType() const { return rendererType; }
	};
}

#endif
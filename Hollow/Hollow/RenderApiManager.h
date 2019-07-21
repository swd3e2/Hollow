#pragma once

#ifndef HW_RENDER_API_MANAGER_H
#define HW_RENDER_API_MANAGER_H

#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "Graphics/Base/RenderApi.h"
#include "Graphics/Platform/DirectX/D3D11RenderApi.h"
#include "Graphics/Platform/OpenGL/OGLRenderApi.h"
#include "Graphics/RendererType.h"

namespace Hollow {
	class RenderApiManager
	{
	public:
		static RenderApi* create(RendererType type, int width, int height)
		{
			switch (type)
			{
				case RendererType::OpenGL: {
					RenderApi::startUp<OGLRenderApi>(width, height);
				} break;
				case RendererType::DirectX: {
					RenderApi::startUp<D3D11RenderApi>(width, height);
				} break;
				case RendererType::None: return nullptr;
			}

			return RenderApi::instance();
		}
	};
}

#endif
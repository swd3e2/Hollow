#pragma once

#ifndef HW_WINDOW_MANAGER_H
#define HW_WINDOW_MANAGER_H

#include "Graphics/Window.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Platform.h"
#include "Graphics/CommonTypes.h"
#include "Hollow/Graphics/Platform/DirectX/D3D11Win32Window.h"
#include "Hollow/Graphics/Platform/OpenGL/Win32/OGLWin32Window.h"

namespace Hollow {
	class WindowManager
	{
	public:
		static Window* create(RendererType rendererType, int width, int height, WindowType type)
		{
			switch (rendererType)
			{
				case RendererType::None: 
					return nullptr;
				case RendererType::OpenGL: {
					OGLWin32Window::startUp<OGLWin32Window>(GetModuleHandle(NULL), width, height, type);
				} break; 
				case RendererType::DirectX: {
					D3D11Win32Window::startUp<D3D11Win32Window>(GetModuleHandle(NULL), width, height, type);
				} break;
			}

			return Window::instance();
		}
	};
}

#endif
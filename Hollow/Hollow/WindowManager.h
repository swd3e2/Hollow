#pragma once

#ifndef HW_WINDOW_MANAGER_H
#define HW_WINDOW_MANAGER_H

#include "Graphics/Renderer/Base/Window.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11Win32Window.h"
#include "Hollow/Graphics/Renderer/OpenGL/Win32/OGLWin32Window.h"
#include "Hollow/Graphics/RendererType.h"

namespace Hollow {
	class WindowManager
	{
	private:
		Window* window;
		RendererType rendererType;
	public:
		WindowManager(RendererType type) : rendererType(type), window(nullptr) {}

		Window* Initialize(int width, int height, WindowType type)
		{
			switch (rendererType)
			{
				case RendererType::None: return nullptr;
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
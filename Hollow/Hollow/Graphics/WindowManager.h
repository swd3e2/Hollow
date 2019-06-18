#pragma once

#ifndef HW_WINDOW_MANAGER_H
#define HW_WINDOW_MANAGER_H

#include "Renderer/Base/Window.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11Win32Window.h"
#include "Hollow/Graphics/Renderer/OpenGL/Win32/OGLWin32Window.h"
#include "Hollow/Graphics/RendererType.h"

namespace Hollow {
	class WindowManager : public CModule<WindowManager>
	{
	private:
		Window* window;
		RendererType rendererType;
	public:
		WindowManager(RendererType type) :
			rendererType(type) 
		{
			setStartedUp(); 
		}

		~WindowManager() 
		{ 
			setShutdown(); 
		}

		Window* Initialize(int width, int height)
		{
			window = nullptr;
			switch (rendererType)
			{
				case RendererType::None: return nullptr;
				case RendererType::OpenGL: return (window = new OGLWin32Window(GetModuleHandle(NULL), width, height));
				case RendererType::DirectX: return (window = new D3D11Win32Window(GetModuleHandle(NULL), width, height));
			}
		}

		void PollEvents() const { window->ProcessMessage(); }
		Window* getWindow() const { return window; }
	};
}

#endif
#pragma once

#ifndef HW_WINDOW_MANAGER_H
#define HW_WINDOW_MANAGER_H

#include "Renderer/Base/Window.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11Win32Window.h"
#include "Hollow/Graphics/Renderer/OpenGL/Win32/OGLWin32Window.h"

namespace Hollow {
	class WindowManager : public CModule<WindowManager>
	{
	private:
		Window* window;
	public:
		WindowManager() { setStartedUp(); }
		~WindowManager() { setShutdown(); }
		Window* Initialize(int width, int height)
		{
			window = nullptr;
#ifdef D3D11
			window = new D3D11Win32Window(GetModuleHandle(NULL), width, height);
#endif
#ifdef OPENGL
			window = new OGLWin32Window(GetModuleHandle(NULL), width, height);
#endif
			return window;
		}

		void PollEvents() { window->ProcessMessage(); }
		Window* getWindow() { return window; }
	};
}

#endif
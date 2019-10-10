#pragma once

#ifndef HW_OGL_WIN32_WINDOW_H
#define HW_OGL_WIN32_WINDOW_H

#include <string>
#include "windows.h"
#include "Hollow/Input/InputManager.h"
#include "Hollow/Platform.h"
#include <Windowsx.h>
#include "Hollow/Events/WindowCreateEvent.h"
#include "Hollow/Events/EventSystem.h"
#include "Hollow/Events/ButtonPressedEvent.h"
#include "Hollow/Events/ButtonReleasedEvent.h"
#include "Hollow/Graphics/Win32Window.h"
#include <memory>
#include "../OGLPrerequisites.h"
#include "../GL/glew.h"
#include "../GL/wglew.h"
#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_win32.h"
#include "Hollow/Common/Logger.h"

LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hollow {
	class OGLWin32Window : public Win32Window
	{
	public:
		OGLWin32Window(HINSTANCE hInst, int width, int height, WindowType type);
	};
}

#endif
#pragma once

#ifndef HW_D3D11_WIN32_WINDOW_H
#define HW_D3D11_WIN32_WINDOW_H

#include "Hollow/Input/InputManager.h"
#include "Hollow/Platform.h"
#include <Windowsx.h>
#include "Hollow/Events/WindowCreateEvent.h"
#include "Hollow/Events/EventSystem.h"
#include "Hollow/Events/ButtonPressedEvent.h"
#include "Hollow/Events/ButtonReleasedEvent.h"
#include "Hollow/Graphics/Renderer/Base/Platform/Win32Window.h"
#include <memory>
#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_win32.h"

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hollow{
	class D3D11Win32Window : public Win32Window
	{
	public:
		D3D11Win32Window(HINSTANCE hInst, int width, int height, WindowType type);
		virtual bool ProcessMessage() override;
		static LRESULT WINAPI _HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI _HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT WINAPI HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}

#endif
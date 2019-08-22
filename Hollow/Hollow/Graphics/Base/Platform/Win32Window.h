#pragma once

#ifndef HW_WIN32_WINDOW_H
#define HW_WIN32_WINDOW_H

#include "../Window.h"
#include "windows.h"
#include <Windowsx.h>
#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_win32.h"
#include "Hollow/Events/WindowCreateEvent.h"
#include "Hollow/Events/EventSystem.h"
#include "Hollow/Events/ButtonPressedEvent.h"
#include "Hollow/Events/ButtonReleasedEvent.h"
#include "Hollow/Events/MouseMoveEvent.h"
#include "Hollow/Input/InputManager.h"
#include <memory>

LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hollow {
	class Win32Window : public Window
	{
	protected:
		HINSTANCE hInst = nullptr;
		HWND hWnd;
	public:
		Win32Window(HINSTANCE hInst, int width, int height, WindowType type);

		inline HWND getHWND() { return hWnd; }
		static LRESULT WINAPI _HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI _HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		virtual bool processMessage() override;
	protected:
		void showWindow();
	};
}

#endif
#pragma once
#include <string>
#include "windows.h"
#include "Hollow/Input/InputManager.h"
#include "Hollow/Platform.h"

namespace Hollow {

	class HOLLOW_API Window
	{
	public:
		Window(HINSTANCE hInst, int width, int height);
		bool ProcessMessage();
		virtual bool CustomFunction(HWND& hWnd, UINT& msg, WPARAM& wParam, LPARAM& lParam);
		static LRESULT WINAPI _HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI _HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT WINAPI HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		HWND * getHWND();
	private:
		void m_UpdateWindowState();
		HINSTANCE hInst = nullptr;
		static HWND hWnd;
	};
}
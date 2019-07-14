#pragma once

#ifndef HW_WIN32_WINDOW_H
#define HW_WIN32_WINDOW_H

#include "../Window.h"
#include "windows.h"

namespace Hollow {
	class Win32Window : public Window
	{
	protected:
		HINSTANCE hInst = nullptr;
		HWND hWnd;
	public:
		Win32Window(HINSTANCE hInst) :
			hInst(hInst)
		{}

		inline HWND* getHWND() { return &hWnd; }
	};
}

#endif
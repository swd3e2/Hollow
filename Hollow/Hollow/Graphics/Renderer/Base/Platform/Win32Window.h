#pragma once

#include "../Window.h"
#include "windows.h"

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
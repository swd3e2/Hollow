#pragma once


#include "Hollow/Graphics/WindowManager.h"
#include "Win32/OGLWin32Window.h"

class OGLWindowManager : public WindowManager
{
public:
	virtual Window* createInternal(int width, int height) override
	{
		return new OGLWin32Window(GetModuleHandle(NULL), width, height);
	}
};
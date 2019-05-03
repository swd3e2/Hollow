#pragma once

#include "Hollow/Graphics/WindowManager.h"
#include "D3D11Win32Window.h"

class HOLLOW_API D3D11WindowManager : public WindowManager
{
public:
	virtual Window* createInternal(int width, int height) override
	{
		return new D3D11Win32Window(GetModuleHandle(NULL), width, height);
	}
};
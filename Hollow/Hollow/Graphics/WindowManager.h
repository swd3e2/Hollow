#pragma once

#include "Renderer/Base/Window.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Platform.h"

class WindowManager : public CModule<WindowManager>
{
private:
	Window* window;
public:
	WindowManager() { setStartedUp(); }
	~WindowManager() { setShutdown(); }
	Window* Initialize(int width, int height)
	{
		window = createInternal(width, height);
		return window;
	}

	virtual Window* createInternal(int width, int height) = 0;
	void PollEvents() { window->ProcessMessage(); }
	Window* getWindow() { return window; }
};
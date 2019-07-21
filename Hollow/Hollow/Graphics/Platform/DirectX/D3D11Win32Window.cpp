#include "D3D11Win32Window.h"

namespace Hollow {
	D3D11Win32Window::D3D11Win32Window(HINSTANCE hInst, int width, int height, WindowType type)
		: Win32Window(hInst, width, height, type)
	{
		showWindow();
	}
}
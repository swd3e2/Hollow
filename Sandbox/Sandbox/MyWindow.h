#pragma once
#include "Hollow/Window/Window.h"
// For handling imgui messages
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class MyWindow : public Hollow::Window
{
public:
	MyWindow(HINSTANCE hInst, int width, int height) : Hollow::Window(hInst, width, height) {}

	virtual bool CustomFunction(HWND& hWnd, UINT& msg, WPARAM& wParam, LPARAM& lParam) override
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;
		return false;
	}
};

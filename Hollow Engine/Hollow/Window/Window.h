#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <string>
#include "windows.h"
#include "Input/InputManager.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "Platform.h"

// For handling imgui messages
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class HOLLOW_API Window
{
public:
	Window(HINSTANCE hInst, int width, int height);
	bool ProcessMessage();
	static LRESULT WINAPI _HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI _HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WINAPI HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND * getHWND();
private:
	void m_UpdateWindowState();
	HINSTANCE hInst = nullptr;
	static HWND hWnd;
};

#endif
#pragma once
#include <string>
#include "windows.h"
#include "Hollow/Input/InputManager.h"
#include "Hollow/Platform.h"
#include <Windowsx.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class HOLLOW_API Win32Window
{
public:
	Win32Window(HINSTANCE hInst, int width, int height);
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

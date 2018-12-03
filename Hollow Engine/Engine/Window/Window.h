#pragma once
#define NOMINMAX
#include <string>
#include "windows.h"

class Window
{
public:
	Window(HINSTANCE hInst, LPWSTR pArgs, int width, int height);
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

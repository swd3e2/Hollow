#pragma once
#include <string>
#include "windows.h"
#include "Hollow/Input/InputManager.h"
#include "Hollow/Platform.h"
#include <Windowsx.h>
#include "Hollow/Events/WindowCreateEvent.h"
#include "Hollow/Events/EventSystem.h"
#include "Hollow/Events/ButtonPressedEvent.h"
#include "Hollow/Events/ButtonReleasedEvent.h"
#include "Hollow/Graphics/Renderer/Base/Window.h"
#include <memory>

class  D3D11Win32Window : public Window
{
public:
	D3D11Win32Window(HINSTANCE hInst, int width, int height);
	virtual bool ProcessMessage() override;
	static LRESULT WINAPI _HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI _HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WINAPI HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND * getHWND();
private:
	void m_UpdateWindowState();
	HINSTANCE hInst = nullptr;
	HWND hWnd;
};

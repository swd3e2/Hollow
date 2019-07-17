#include "OGLWin32Window.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	//if (severity != 33387) {
		HW_ERROR("GL CALLBACK:: {} type = 0x{}x, severity = 0x{}x, message = {}", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
		Hollow::Logger::instance()->log("GL CALLBACK:: {} type = 0x{}x, severity = 0x{}x, message = {}", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
	//}
}

namespace Hollow {
	OGLWin32Window::OGLWin32Window(HINSTANCE hInst, int width, int height, WindowType type)
		: Win32Window(hInst)
	{
		// Creating window class
		WNDCLASSEX windowClass = {};
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_CLASSDC;
		windowClass.lpfnWndProc = _HandleMsgSetup;
		windowClass.cbClsExtra = 0;
		windowClass.hInstance = hInst;
		windowClass.hIcon = nullptr;
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = nullptr;
		windowClass.lpszMenuName = nullptr;
		windowClass.lpszClassName = "HollowAppClass";
		RegisterClassEx(&windowClass);

		// Center positions
		int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
		int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

		// Creating rectangle
		RECT windowRect;
		windowRect.left = centerScreenX;
		windowRect.right = width + windowRect.left;
		windowRect.top = centerScreenY;
		windowRect.bottom = height + windowRect.top;
		AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		hWnd = CreateWindow("HollowAppClass", "Hollow", WS_POPUPWINDOW, /* WS_POPUP*/
			windowRect.left, windowRect.top,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			nullptr, nullptr, hInst, this);

		RAWINPUTDEVICE Rid[2];

		Rid[0].usUsagePage = 0x01;
		Rid[0].usUsage = 0x02;
		Rid[0].dwFlags = 0;   // adds HID mouse and also ignores legacy mouse messages
		Rid[0].hwndTarget = 0;

		RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		HDC hdc = GetDC(hWnd);

		int  suggestedPixelFormatIndex = 0;
		GLuint extendedPick = 0;
		suggestedPixelFormatIndex = ChoosePixelFormat(hdc, &pfd);

		PIXELFORMATDESCRIPTOR suggestedPixelFormat = {};
		DescribePixelFormat(hdc, suggestedPixelFormatIndex, sizeof(PIXELFORMATDESCRIPTOR), &suggestedPixelFormat);
		SetPixelFormat(hdc, suggestedPixelFormatIndex, &suggestedPixelFormat);

		HGLRC tempContext = wglCreateContext(hdc);
		wglMakeCurrent(hdc, tempContext);

		GLenum error;
		if (error = glewInit())
		{
			HW_ERROR("{}", glewGetErrorString(error));
		}

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);

		const int pixelAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_ALPHA_BITS_ARB, 8,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB, 4,
			0
		};

		hdc = GetDC(hWnd);

		int pixelFormatID;
		UINT numFormats;
		bool status = wglChoosePixelFormatARB(hdc, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
		PIXELFORMATDESCRIPTOR PFD;
		DescribePixelFormat(hdc, pixelFormatID, sizeof(PFD), &PFD);
		SetPixelFormat(hdc, pixelFormatID, &PFD);

		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 6,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
			0
		};

		HGLRC hrc = wglCreateContextAttribsARB(hdc, 0, attribs);

		wglMakeCurrent(hdc, hrc);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}

	LRESULT WINAPI OGLWin32Window::_HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			OGLWin32Window* const pWnd = reinterpret_cast<OGLWin32Window*>(pCreate->lpCreateParams);
			// set WinAPI-managed user data to store ptr to window class
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&OGLWin32Window::_HandleMsgThunk));
			// forward message to window class handler
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		// if we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT WINAPI OGLWin32Window::_HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// retrieve ptr to window class
		OGLWin32Window* const pWnd = reinterpret_cast<OGLWin32Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT OGLWin32Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
			return true;
		}

		LRESULT result = 0;
		switch (msg)
		{
		case WM_INPUT:
		{
			UINT dataSize;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
			if (dataSize > 0)
			{
				std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);
				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
				{
					RAWINPUT* input = reinterpret_cast<RAWINPUT*>(rawData.get());
					if (input->header.dwType == RIM_TYPEMOUSE)
					{
						InputManager::mx = input->data.mouse.lLastX;
						InputManager::my = input->data.mouse.lLastY;
					}
				}
			}
		} break;
		case WM_CLOSE:
		{
			setIsClosed(true);
		} break;
		case WM_KEYDOWN:
		{
			EventSystem::instance()->addEvent(new ButtonPressedEvent(static_cast<eKeyCodes>(wParam)));
		} break;
		case WM_KEYUP:
		{
			EventSystem::instance()->addEvent(new ButtonReleasedEvent(static_cast<eKeyCodes>(wParam)));
		} break;
		// Mouse events handling
		case WM_LBUTTONDOWN:
		{
			InputManager::SetMouseButtonActive(eMouseKeyCodes::MOUSE_LEFT, true);
		} break;
		case WM_RBUTTONDOWN:
		{
			InputManager::SetMouseButtonActive(eMouseKeyCodes::MOUSE_RIGHT, true);
		} break;
		case WM_MBUTTONDOWN:
		{
			InputManager::SetMouseButtonActive(eMouseKeyCodes::MOUSE_MIDDLE, true);
		} break;
		case WM_LBUTTONUP:
		{
			InputManager::SetMouseButtonActive(eMouseKeyCodes::MOUSE_LEFT, false);
		} break;
		case WM_RBUTTONUP:
		{
			InputManager::SetMouseButtonActive(eMouseKeyCodes::MOUSE_RIGHT, false);
		} break;
		case WM_MBUTTONUP:
		{
			InputManager::SetMouseButtonActive(eMouseKeyCodes::MOUSE_MIDDLE, false);
		} break;
		case WM_MOUSEMOVE:
		{
			InputManager::mcx = GET_X_LPARAM(lParam);
			InputManager::mcy = GET_Y_LPARAM(lParam);
		} break;
		default:
			result = DefWindowProc(hWnd, msg, wParam, lParam);
			break;
		}
		return result;
	}

	bool OGLWin32Window::ProcessMessage()
	{
		MSG message;
		while (PeekMessage(&message, *getHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		return true;
	}
}

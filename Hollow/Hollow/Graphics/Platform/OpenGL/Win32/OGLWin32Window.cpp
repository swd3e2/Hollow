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
	if (severity != 33387) {
		HW_ERROR("GL CALLBACK:: {} type = 0x{}x, severity = 0x{}x, message = {}", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
		Hollow::Logger::instance()->log("GL CALLBACK:: {} type = 0x{}x, severity = 0x{}x, message = {}", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
	}
}

namespace Hollow {
	OGLWin32Window::OGLWin32Window(HINSTANCE hInst, int width, int height, WindowType type)
		:Win32Window(hInst, width, height, type)
	{
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
		wglSwapIntervalEXT(0);
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		showWindow();
	}
}

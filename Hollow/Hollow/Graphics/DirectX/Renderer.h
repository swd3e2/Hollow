#pragma once
#include "HollowRenderer.h"
#include <windows.h>
#include "DirectXRenderer.h"
#include "Hollow/Platform.h"

namespace Hollow {
	enum RendererType {
		DirectX,
		OpenGL
	};

	class HOLLOW_API Renderer
	{
	private:
		static HollowRenderer* rendererInstance;
	public:
		static void InitializeRenderer(RendererType type, HWND* hwnd, int height, int width);
		static HollowRenderer* Get();
	};
}
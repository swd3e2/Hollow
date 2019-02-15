#include "Renderer.h"

namespace Hollow {
	HollowRenderer* Renderer::rendererInstance = nullptr;

	void Renderer::InitializeRenderer(RendererType type, HWND * hwnd, int width, int height)
	{
		switch (type)
		{
		case Hollow::DirectX:
			if (rendererInstance == nullptr) {
				rendererInstance = new Core::Graphics::DirectXRenderer(hwnd, width, height);
			}
			break;
		case Hollow::OpenGL:
			throw std::exception("Not supported!");
			break;
		default:
			break;
		}
	}

	HollowRenderer * Renderer::Get()
	{
		return rendererInstance;
	}
}
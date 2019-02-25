#include "HollowRenderer.h"

namespace Hollow
{
	HollowRenderer* HollowRenderer::instance = nullptr;

	HollowRenderer::HollowRenderer(HWND * hwnd, UINT width, UINT height) :
		hwnd(hwnd), width(width), height(height)
	{
		if (instance != nullptr) {
			throw std::exception("Can't create more than one renderer!");
		}
		instance = this;
	}

	HollowRenderer * HollowRenderer::Get()
	{
		if (instance == nullptr) {
			throw std::exception("Renderer not initialized!");
		}
		return instance;
	}
}
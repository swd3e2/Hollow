#pragma once

#include "Hollow/Graphics/RenderTargetManager.h"
#include "D3D11Prerequisites.h"
#include "D3D11RenderTarget.h"

namespace Hollow {
	class D3D11RenderTargetManager : public RenderTargetManager
	{
	public:
		virtual RenderTarget* create(int width, int height) override;
	private:
		DXGI_FORMAT GetDepthResourceFormat(DXGI_FORMAT depthformat);
		DXGI_FORMAT GetDepthSRVFormat(DXGI_FORMAT depthformat);
	};
}
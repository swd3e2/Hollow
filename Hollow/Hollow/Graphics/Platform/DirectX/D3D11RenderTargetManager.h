#pragma once

#include "Hollow/Graphics/RenderTargetManager.h"
#include "D3D11Prerequisites.h"
#include "D3D11RenderTarget.h"

namespace Hollow {
	class D3D11RenderTargetManager : public RenderTargetManager
	{
	public:
		virtual s_ptr<RenderTarget> create(RENDER_TARGET_DESC desc) override;
	private:
		DXGI_FORMAT getDepthResourceFormat(DXGI_FORMAT depthformat);
		DXGI_FORMAT getDepthSRVFormat(DXGI_FORMAT depthformat);
		DXGI_FORMAT getTextureFormat(RENDER_TARGET_TEXTURE_FORMAT format);
	};
}
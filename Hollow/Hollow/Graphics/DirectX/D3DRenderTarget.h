#pragma once
#include <d3d11.h>
#include "Hollow/Platform.h"
#include "Hollow/Common/Log.h"

class HOLLOW_API D3DRenderTarget
{
public:
	D3DRenderTarget(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain, int width, int height);
	~D3DRenderTarget();
	inline ID3D11RenderTargetView * GetMainRenderTaget() { return mainRenderTarget; }
	inline ID3D11RenderTargetView ** GetAddressOfMainRenderTaget() { return &mainRenderTarget; }
private:
	ID3D11RenderTargetView*		mainRenderTarget;
	ID3D11Texture2D*			m_BackBuffer;
};

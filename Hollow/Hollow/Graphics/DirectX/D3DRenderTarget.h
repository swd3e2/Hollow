#pragma once
#include <d3d11.h>
#include "Hollow/Platform.h"
#include "Hollow/Common/Log.h"

enum RenderTargetType
{
	MAIN,
	SECONDARY
};

class HOLLOW_API D3DRenderTarget
{
public:
	D3DRenderTarget(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain, int width, int height, RenderTargetType type);
	~D3DRenderTarget();
	inline ID3D11RenderTargetView * GetRenderTaget() { return renderTarget; }
	inline ID3D11RenderTargetView ** GetAddressOfRenderTaget() { return &renderTarget; }
	inline ID3D11ShaderResourceView * GetShaderResourceView() { return m_ShaderResourceView; }
	inline ID3D11ShaderResourceView ** GetAddressOfShaderResourceView() { return &m_ShaderResourceView; }
private:
	ID3D11RenderTargetView*		renderTarget;
	ID3D11Texture2D*			m_BackBuffer;
	ID3D11ShaderResourceView*	m_ShaderResourceView;
};

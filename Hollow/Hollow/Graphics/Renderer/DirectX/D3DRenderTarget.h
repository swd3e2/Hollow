#pragma once
#include <d3d11.h>
#include "Hollow/Platform.h"
#include "Hollow/Common/Log.h"
#include "D3DContext.h"
#include "D3DRenderer.h"

enum HOLLOW_API RenderTargetType
{
	MAIN,
	SECONDARY
};

class HOLLOW_API D3DRenderTarget
{
public:
	D3DRenderTarget(
		int width, 
		int height, 
		RenderTargetType type, 
		DXGI_FORMAT format, 
		IDXGISwapChain* swapChain = nullptr
	);

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

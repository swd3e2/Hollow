#pragma once
#include "Hollow/Platform.h"
#include <d3d11.h>
#include <wrl/client.h>
#include "Hollow/Common/Log.h"

class HOLLOW_API D3DRenderTarget
{
public:
	D3DRenderTarget(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain, int width, int height);
	inline ID3D11RenderTargetView * GetMainRenderTaget() { return mainRenderTarget.Get(); }
	inline ID3D11RenderTargetView ** GetAddressOfMainRenderTaget() { return mainRenderTarget.GetAddressOf(); }

	inline ID3D11RenderTargetView * GetSecondRenderTaget() { return secondRenderTarget.Get(); }
	inline ID3D11RenderTargetView ** GetAddressOfSecondRenderTaget() { return secondRenderTarget.GetAddressOf(); }

	inline ID3D11ShaderResourceView * GetShaderResourceView() { return m_ShaderResourceView.Get(); }
	inline ID3D11ShaderResourceView ** GetAddressOfShaderResourceView() { return m_ShaderResourceView.GetAddressOf(); }
private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		mainRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		secondRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_ShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_BackBuffer;
};

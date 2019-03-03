#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"

class HOLLOW_API D3DDepthStencil
{
public:
	D3DDepthStencil(ID3D11Device * device, int width, int height, DXGI_FORMAT format, int samplecount);

	inline ID3D11DepthStencilView * GetDepthStencilView() { return m_DepthStencilView.Get(); }
	inline ID3D11DepthStencilView ** GetAddressOfDepthStencilView() { return m_DepthStencilView.GetAddressOf(); }

	inline ID3D11DepthStencilState * GetDepthStencilState() { return m_DepthStencilState.Get(); }
	inline ID3D11ShaderResourceView * GetDepthStencilResource() { return mDepthResourceView; }
	inline ID3D11ShaderResourceView ** GetAddressOfDepthStencilResource() { return &mDepthResourceView; }
private:
	DXGI_FORMAT GetDepthResourceFormat(DXGI_FORMAT depthformat);
	DXGI_FORMAT GetDepthSRVFormat(DXGI_FORMAT depthformat);
private:
	float m_zNear, m_zFar;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_DepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_DepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		m_DepthStencilState;
	ID3D11ShaderResourceView*							mDepthResourceView;
};

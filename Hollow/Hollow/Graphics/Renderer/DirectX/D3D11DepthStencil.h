#pragma once

#ifndef HW_D3D11_DEPTH_STENCIL_H
#define HW_D3D11_DEPTH_STENCIL_H

#include "D3D11Prerequisites.h"

class HOLLOW_API D3D11DepthStencil
{
public:
	D3D11DepthStencil(int width, int height, DXGI_FORMAT format, int samplecount);
	~D3D11DepthStencil();
	inline ID3D11DepthStencilView * GetDepthStencilView() { return m_DepthStencilView; }
	inline ID3D11DepthStencilView ** GetAddressOfDepthStencilView() { return &m_DepthStencilView; }

	inline ID3D11DepthStencilState * GetDepthStencilState() { return m_DepthStencilState; }
	inline ID3D11ShaderResourceView * GetDepthStencilResource() { return mDepthResourceView; }
	inline ID3D11ShaderResourceView ** GetAddressOfDepthStencilResource() { return &mDepthResourceView; }
private:
	DXGI_FORMAT GetDepthResourceFormat(DXGI_FORMAT depthformat);
	DXGI_FORMAT GetDepthSRVFormat(DXGI_FORMAT depthformat);
private:
	float m_zNear, m_zFar;
	ID3D11DepthStencilView*		m_DepthStencilView;
	ID3D11Texture2D*			m_DepthStencilBuffer;
	ID3D11DepthStencilState*	m_DepthStencilState;
	ID3D11ShaderResourceView*	mDepthResourceView;
};

#endif
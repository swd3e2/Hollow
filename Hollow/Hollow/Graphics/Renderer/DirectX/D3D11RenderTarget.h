#pragma once

#ifndef HW_D3D11_RENDER_TARGET_H
#define HW_D3D11_RENDER_TARGET_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Renderer/Base/RenderTarget.h"

enum  RenderTargetType
{
	MAIN,
	SECONDARY
};

class  D3D11RenderTarget : public RenderTarget
{
public:
	D3D11RenderTarget(
		int width, 
		int height, 
		RenderTargetType type, 
		DXGI_FORMAT format
	);

	~D3D11RenderTarget();
	inline ID3D11RenderTargetView * GetRenderTaget() { return renderTarget; }
	inline ID3D11RenderTargetView ** GetAddressOfRenderTaget() { return &renderTarget; }
	inline ID3D11ShaderResourceView * GetShaderResourceView() { return m_ShaderResourceView; }
	inline ID3D11ShaderResourceView ** GetAddressOfShaderResourceView() { return &m_ShaderResourceView; }
private:
	ID3D11RenderTargetView*		renderTarget;
	ID3D11Texture2D*			m_BackBuffer;
	ID3D11ShaderResourceView*	m_ShaderResourceView;
};

#endif
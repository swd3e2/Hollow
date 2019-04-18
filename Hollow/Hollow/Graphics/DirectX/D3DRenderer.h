#pragma once

#ifndef HW_D3D_RENDERER_H
#define HW_D3D_RENDERER_H

#include <d3d11.h>
#include "Hollow/Common/Log.h"
#include "D3DRenderTarget.h"
#include "D3DDepthStencil.h"
#include "D3DPixelShader.h"
#include "D3DVertexShader.h"
#include "D3DConstantBuffer.h"
#include "Hollow/Graphics/IRenderer.h"
#include "D3DRenderable.h"
#include "D3DBlendState.h"
#include "D3DSamplerState.h"
#include "D3DRasterizerState.h"
#include "Hollow/Graphics/Window/Win32/Win32Window.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Platform.h"
#include "Hollow/Containers/vector.h"
#include "D3DRenderable.h"
#include "D3DConstBufferMapping.h"
#include "Icons/LightIcon.h"
#include "Hollow/Resources/ShaderManager.h"
#include "Hollow/Graphics/PointLight.h"
#include "Hollow/Graphics/DirectionalLight.h"
#include "Hollow/Graphics/ShadowMap.h"
#include "D3DMaterial.h"
#include "Hollow/Core/CModule.h"

// Simple directx renderer
class HOLLOW_API D3DRenderer : public IRenderer, public CModule<D3DRenderer>
{
public:
	bool vSync = true;
private:					
	ID3D11Device*			m_Device;
	ID3D11DeviceContext*	m_DeviceContext;
	IDXGISwapChain*			m_SwapChain;
	D3DRasterizerState*		m_rasterizerState;

	int width;
	int height;
	ID3D11ShaderResourceView *const pSRV[1] = { NULL };
	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
private:
public:
	D3DRenderer(int width, int height, HWND* hwnd);
	~D3DRenderer();

	inline void SetShaderResource(UINT slot, ID3D11ShaderResourceView * shaderResourceView)
	{ 
		m_DeviceContext->PSSetShaderResources(slot, 1, &shaderResourceView);
	}
		
	inline void FreeShaderResource(UINT slot)
	{
		m_DeviceContext->PSSetShaderResources(slot, 1, pSRV);
	}

	inline void SetVertexShader(D3DVertexShader* vs)
	{ 
		m_DeviceContext->VSSetShader(vs->GetShader(), NULL, 0); 
		m_DeviceContext->IASetInputLayout(vs->GetInputLayout());
	}
	inline void SetPixelShader(D3DPixelShader* ps)
	{ 
		m_DeviceContext->PSSetShader(ps->GetShader(), NULL, 0); 
	}
	
	inline void ClearDepthStencilView(D3DDepthStencil*	ds, int flag) 
	{ 
		m_DeviceContext->ClearDepthStencilView(ds->GetDepthStencilView(), flag, 1.0f, 0);
	}

	inline void SetContantBuffer(UINT slot, D3DConstantBuffer* cb) 
	{ 
		m_DeviceContext->VSSetConstantBuffers(slot, 1, cb->GetAddressOf());
		m_DeviceContext->PSSetConstantBuffers(slot, 1, cb->GetAddressOf());
	}

	inline void SetVertexBuffer(D3DBuffer* buffer)
	{
		m_DeviceContext->IASetVertexBuffers(0, 1, buffer->GetAddressOf(), buffer->StridePtr(), &this->offset);
	}

	inline void ClearRenderTargetView(D3DRenderTarget* renderTarget, float* color)
	{
		m_DeviceContext->ClearRenderTargetView(renderTarget->GetRenderTaget(), color);
	}

	inline void SetSampler(int slot, D3DSamplerState* sampler)
	{
		m_DeviceContext->PSSetSamplers(slot, 1, sampler->GetSamplerState());
	}

	inline void SetDepthStencil(D3DDepthStencil* depthStencil)
	{
		m_DeviceContext->OMSetDepthStencilState(depthStencil->GetDepthStencilState(), 0);
	}

	inline void SetRenderTarget(D3DRenderTarget* renderTarget, D3DDepthStencil* depthStencil)
	{
		m_DeviceContext->OMSetRenderTargets(1, renderTarget->GetAddressOfRenderTaget(), depthStencil->GetDepthStencilView());
	}

	inline void DrawIndexed(UINT count) { m_DeviceContext->DrawIndexed(count, 0, 0); }
	inline void Draw(UINT count) { m_DeviceContext->Draw(count, 0); }

	inline ID3D11Device* getDevice() { return m_Device; }
	inline ID3D11DeviceContext* getDeviceContext() { return m_DeviceContext; }
	inline IDXGISwapChain* getSwapChain() { return m_SwapChain; }

	void toggleVSync() { vSync = !vSync; }
};

#endif
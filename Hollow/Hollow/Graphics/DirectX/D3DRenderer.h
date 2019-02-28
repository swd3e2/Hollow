#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Hollow/Common/Log.h"
#include "D3DRenderTarget.h"
#include "D3DDepthStencil.h"
#include "D3DPixelShader.h"
#include "D3DVertexShader.h"
#include "D3DIndexBuffer.h"
#include "D3DConstantBuffer.h"
#include "D3DBuffer.h"
#include "Hollow/Graphics/IRenderer.h"
#include "D3DRenderable.h"
#include "D3DBlendState.h"
#include "D3DSamplerState.h"
#include "D3DRasterizerState.h"
#include "Hollow/Graphics/Window/Win32/Win32Window.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Platform.h"
#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 900

using namespace DirectX;

struct WVP
{
	DirectX::XMMATRIX WVP;
};

struct Transform
{
	DirectX::XMMATRIX transform;
	float id;
	bool selected;

};

// Simple directx renderer
class HOLLOW_API D3DRenderer : public IRenderer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device>				m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>				m_SwapChain;

	WVP						m_wvp;
	D3DConstantBuffer*		m_WVPConstantBuffer;
	D3DConstantBuffer*		m_TransformConstantBuffer;
	D3DBlendState*			m_BlendStateTransparancy;
	D3DSamplerState*		m_SamplerStateWrap;
	D3DSamplerState*		m_SamplerStateClamp;
	D3DRenderTarget*		m_RenderTarget;
	D3DDepthStencil*		m_DepthStencil;
	Camera*					camera;
	Win32Window				window;
	int width;
	int height;
	D3D11_VIEWPORT	vp;
	ID3D11ShaderResourceView *const pSRV[1] = { NULL };
	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
private:
public:
	D3DRenderer();
	~D3DRenderer();

	inline void SetVertexBuffer(D3DBuffer* vb) 
	{
		this->m_DeviceContext->IASetVertexBuffers(0, 1, vb->GetAddressOf(), vb->StridePtr(), &this->offset); 
	}

	inline void SetBuffers(D3DBuffer* vb, D3DBuffer* ib)
	{
		this->SetVertexBuffer(vb);
	}

	inline void SetShaderResource(UINT slot, ID3D11ShaderResourceView * shaderResourceView)
	{ 
		this->m_DeviceContext->PSSetShaderResources(slot, 1, &shaderResourceView);
	}
		
	inline void FreeShaderResource(UINT slot)
	{
		this->m_DeviceContext->PSSetShaderResources(slot, 1, pSRV);
	}

	inline void SetVertexShader(D3DVertexShader* vs)
	{ 
		this->m_DeviceContext->VSSetShader(vs->GetShader(), NULL, 0); 
		this->m_DeviceContext->IASetInputLayout(vs->GetInputLayout());

	}
	inline void SetPixelShader(D3DPixelShader* ps)
	{ 
		this->m_DeviceContext->PSSetShader(ps->GetShader(), NULL, 0); 
	}
		
	inline void ClearRenderTarget(ID3D11RenderTargetView*	rt)
	{ 
		this->m_DeviceContext->ClearRenderTargetView(rt, ClearColor); 
	}

	inline void ClearDepthStencil(D3DDepthStencil*	ds) 
	{ 
		this->m_DeviceContext->ClearDepthStencilView(ds->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); 
	}

	inline void SetContantBuffer(UINT slot, D3DConstantBuffer* cb) 
	{ 
		this->m_DeviceContext->VSSetConstantBuffers(slot, 1, cb->GetAddressOf());
		this->m_DeviceContext->PSSetConstantBuffers(slot, 1, cb->GetAddressOf());
	}

	inline void DrawIndexed(UINT count) { m_DeviceContext->DrawIndexed(count, 0, 0); }
	inline void Draw(UINT count) { m_DeviceContext->Draw(count, 0); }
	inline void Present() { this->m_SwapChain->Present(1, 0); }

	void PreUpdateFrame()
	{
		m_DeviceContext->ClearRenderTargetView(m_RenderTarget->GetMainRenderTaget(), ClearColor);
		m_DeviceContext->ClearDepthStencilView(m_DepthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_DeviceContext->PSSetSamplers(0, 1, m_SamplerStateWrap->GetSamplerState());
		m_DeviceContext->PSSetSamplers(1, 1, m_SamplerStateClamp->GetSamplerState());
		m_DeviceContext->OMSetRenderTargets(1, m_RenderTarget->GetAddressOfMainRenderTaget(), m_DepthStencil->GetDepthStencilView());

		UpdateWVP();
	}

	virtual void Draw(IRenderable& renderable) override
	{
		this->SetVertexBuffer(static_cast<D3DBuffer*>(renderable.GetBuffer()));
		m_DeviceContext->Draw(3, 0);
	}

	virtual void PostUpdateFrame() override
	{
		m_SwapChain->Present(1, 0);
	}

	virtual size_t createRenderable(Mesh* mesh) override
	{
		return 1;
	}

	void UpdateWVP()
	{
		m_wvp.WVP = XMMatrixTranspose(XMMatrixIdentity()
			* camera->GetViewMatrix()
			* camera->GetProjectionMatrix());

		m_WVPConstantBuffer->Update(&m_wvp, sizeof(m_wvp));
	}

	ID3D11Device* getDevice()
	{
		return m_Device.Get();
	}

	ID3D11DeviceContext* getDeviceContext()
	{
		return m_DeviceContext.Get();
	}

	virtual bool processMessage() override
	{
		return window.ProcessMessage();
	}
};

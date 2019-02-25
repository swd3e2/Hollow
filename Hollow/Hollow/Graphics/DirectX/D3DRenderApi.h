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
#include "D3DVertexBuffer.h"
#include "Hollow/Graphics/IRenderApi.h"

namespace Hollow {
	class RenderEngine;
	// Simple directx renderer
	class HOLLOW_API D3DRenderApi : public IRenderApi
	{
	friend class RenderEngine;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device>				m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain>				m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_RasterizerState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>			m_SamplerStateWrap;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>			m_SampleStateClamp;
		Microsoft::WRL::ComPtr<ID3D11BlendState>			Transparency;

		RenderTarget*	renderTarget;
		DepthStencil*	depthStencil;

		ID3D11ShaderResourceView *const pSRV[1] = { NULL };
		const UINT offset = 0;
		const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	private:
	public:
		D3DRenderApi(HWND* hwnd, int width, int height);
		~D3DRenderApi();

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

		inline void SetVertexShader(VertexShader* vs) 
		{ 
			this->m_DeviceContext->VSSetShader(vs->GetShader(), NULL, 0); 
			this->m_DeviceContext->IASetInputLayout(vs->GetInputLayout());

		}
		inline void SetPixelShader(PixelShader* ps) 
		{ 
			this->m_DeviceContext->PSSetShader(ps->GetShader(), NULL, 0); 
		}
		
		inline void ClearRenderTarget(ID3D11RenderTargetView*	rt)
		{ 
			this->m_DeviceContext->ClearRenderTargetView(rt, ClearColor); 
		}

		inline void ClearDepthStencil(DepthStencil*	ds) 
		{ 
			this->m_DeviceContext->ClearDepthStencilView(ds->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); 
		}

		inline void SetContantBuffer(UINT slot, D3DConstantBuffer* cb) 
		{ 
			this->m_DeviceContext->VSSetConstantBuffers(slot, 1, cb->GetAddressOf());
			this->m_DeviceContext->PSSetConstantBuffers(slot, 1, cb->GetAddressOf());
		}

		inline void SetSecondRenderTarget()
		{
			this->m_DeviceContext->OMSetRenderTargets(1, this->renderTarget->GetAddressOfSecondRenderTaget(), depthStencil->GetDepthStencilView());
		}

		inline void SetMainRenderTarget()
		{
			this->m_DeviceContext->OMSetRenderTargets(1, this->renderTarget->GetAddressOfMainRenderTaget(), depthStencil->GetDepthStencilView());
		}

		inline void DrawIndexed(UINT count) { m_DeviceContext->DrawIndexed(count, 0, 0); }
		inline void Draw(UINT count) { m_DeviceContext->Draw(count, 0); }
		inline void Present() { this->m_SwapChain->Present(1, 0); }

		virtual void Draw(IRenderable& renderable) override
		{
			this->m_DeviceContext->ClearRenderTargetView(this->renderTarget->GetMainRenderTaget(), ClearColor);
			this->m_DeviceContext->ClearDepthStencilView(this->depthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			this->m_DeviceContext->PSSetSamplers(0, 1, m_SamplerStateWrap.GetAddressOf());
			this->m_DeviceContext->PSSetSamplers(1, 1, m_SampleStateClamp.GetAddressOf());
			this->m_DeviceContext->OMSetRenderTargets(1, this->renderTarget->GetAddressOfMainRenderTaget(), depthStencil->GetDepthStencilView());

		}
	};
}
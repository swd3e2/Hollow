#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Hollow/Common/Log.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "Shaders/PixelShader.h"
#include "Shaders/VertexShader.h"
#include "BufferTemplate/IndexBuffer.h"
#include "BufferTemplate/ConstantBuffer.h"
#include "BufferTemplate/VertexBuffer.h"
namespace Hollow { namespace Core { namespace Graphics {

	// Simple directx renderer
	class HOLLOW_API HollowDirectXRenderer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Device>				m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain>				m_pSwapChain;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_RasterizerState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>			m_SamplerStateWrap;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>			m_SampleStateClamp;
		Microsoft::WRL::ComPtr<ID3D11BlendState>			Transparency;

		RenderTarget*	renderTarget;
		DepthStencil*	depthStencil;
		UINT			width;
		UINT			height;
		
		ID3D11ShaderResourceView *const pSRV[1] = { NULL };
		const UINT offset = 0;
		const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	private:
	public:
		HollowDirectXRenderer(HWND* hwnd, int width, int height);
		~HollowDirectXRenderer();

		template<class T>
		inline void SetVertexBuffer(VertexBuffer<T>* vb) { this->m_DeviceContext->IASetVertexBuffers(0, 1, vb->GetAddressOf(), vb->StridePtr(), &this->offset); }

		template<class T>
		inline void SetIndexBuffer(IndexBuffer<T>* ib) { this->m_DeviceContext->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0); }

		template<class T, class U>
		inline void SetBuffers(VertexBuffer<T>* vb, IndexBuffer<U>* ib) 
		{
			this->SetVertexBuffer(vb);
			this->SetIndexBuffer(ib);
		}

		inline void SetVertexShader(VertexShader* vs) 
		{ 
			this->m_DeviceContext->VSSetShader(vs->GetShader(), NULL, 0); 
			this->m_DeviceContext->IASetInputLayout(vs->GetInputLayout());

		}
		inline void SetPixelShader(PixelShader* ps) { this->m_DeviceContext->PSSetShader(ps->GetShader(), NULL, 0); }
		
		inline void ClearRenderTarget(RenderTarget*	rt) { this->m_DeviceContext->ClearRenderTargetView(rt->GetMainRenderTaget(), ClearColor); }
		inline void CleraDepthStencil(DepthStencil*	ds) { this->m_DeviceContext->ClearDepthStencilView(ds->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); }

		inline void Clear() 
		{
			this->ClearRenderTarget(this->renderTarget);
			this->CleraDepthStencil(this->depthStencil);
		}

		template<class T>
		inline void SetContantBuffer(UINT slot, ConstantBuffer<T>* cb) { this->m_DeviceContext->VSSetConstantBuffers(slot, 1, cb->GetAddressOf()); }

		inline void PreUpdateFrame()
		{
			m_DeviceContext->OMSetRenderTargets(1, this->renderTarget->GetAddressOfMainRenderTaget(), this->depthStencil->GetDepthStencilView());
			m_DeviceContext->RSSetState(this->m_RasterizerState.Get());
		}

		inline void Draw(UINT count) { m_DeviceContext->DrawIndexed(count, 0, 0); }
		inline void Present() { this->m_pSwapChain->Present(1, 0); }

		inline ID3D11Device* GetDevice() const { return this->m_Device.Get(); }
		inline ID3D11DeviceContext* GetDeviceContext() const { return this->m_DeviceContext.Get(); }
	};

} } }
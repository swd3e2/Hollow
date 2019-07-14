#pragma once

#ifndef HW_D3D11_RENDER_API_H
#define HW_D3D11_RENDER_API_H

#include "Hollow/Graphics/Base/RenderApi.h"
#include "D3D11Prerequisites.h"
#include "Hollow/Common/Log.h"
namespace Hollow {
	class D3D11RenderApi : public RenderApi
	{
	private:
		D3D11Context* context;

		int width;
		int height;

		D3D11SamplerState* m_SamplerStateWrap;
		D3D11SamplerState* m_SamplerStateClamp;

		ID3D11RenderTargetView* renderTarget;
		ID3D11Texture2D* m_BackBuffer;

		ID3D11DepthStencilView* m_DepthStencilView;

		ID3D11DepthStencilState* m_DepthStencilStateNever;
		ID3D11DepthStencilState* m_DepthStencilStateLess;
		ID3D11DepthStencilState* m_DepthStencilStateEqual;
		ID3D11DepthStencilState* m_DepthStencilStateLequal;
		ID3D11DepthStencilState* m_DepthStencilStateGreater;
		ID3D11DepthStencilState* m_DepthStencilStateNotEqual;
		ID3D11DepthStencilState* m_DepthStencilStateAlways;

		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11ShaderResourceView* m_DepthResourceView;

		D3D11_VIEWPORT vp;

		ID3D11ShaderResourceView* const pSRV[1] = { NULL };
		ID3D11UnorderedAccessView* pUAV[1] = { NULL };
		ID3D11RenderTargetView* nullRTV[3] = { NULL };

		D3D11RasterizerState* m_cullNone;
		D3D11RasterizerState* m_cullFront;
		D3D11RasterizerState* m_cullBack;

		D3D11BlendState* blendState;
		const UINT uavs = 0;
		const UINT offset = 0;
	public:
		D3D11RenderApi(int width, int height);
		~D3D11RenderApi();

		virtual void onStartUp() override;

		virtual void SetIndexBuffer(IndexBuffer* buffer) override;
		virtual void SetVertexBuffer(VertexBuffer* buffer) override;
		virtual void SetTexture(UINT slot, Texture* texture) override;
		virtual void UnsetTexture(UINT) override;
		virtual void SetTextureColorBuffer(UINT slot, RenderTarget* renderTarget, UINT targetNum) override;
		virtual void SetTextureDepthBuffer(UINT slot, RenderTarget* renderTarget) override;
		virtual void SetShader(ShaderProgram* shader) override;
		virtual void Present() override;
		virtual void Draw(UINT count) override;
		virtual void DrawIndexed(UINT count) override;
		virtual void SetGpuBuffer(GPUBuffer*) override;
		virtual void SetViewport(int w0, int y0, int w, int y) override;
		virtual void SetRenderTarget(RenderTarget* renderTarget) override;
		virtual void ClearRenderTarget(RenderTarget* renderTarget, const float* color) override;
		virtual void SetDepthTestFunction(DEPTH_TEST_FUNCTION func) override;
		virtual void SetCullMode(CULL_MODE mode) override;


		void SetSampler(int slot, D3D11SamplerState* sampler);
		void SetRasterizerState(D3D11RasterizerState* rasterizer);
		void SetBlendState(D3D11BlendState* blend, float* factor, unsigned int mask);

		virtual void SetLayout(const INPUT_LAYOUT_DESC& desc) override;

		virtual InputLayout* CreateLayout(const INPUT_LAYOUT_DESC& desc) override;
		D3D11Context& getContext() { return *context; }
	};
}

#endif
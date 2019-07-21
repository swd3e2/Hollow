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

		ID3D11RenderTargetView* m_RenderTarget;
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

		D3D11RasterizerState* m_CullNone;
		D3D11RasterizerState* m_CullFront;
		D3D11RasterizerState* m_CullBack;

		D3D11BlendState* blendState;
		const UINT uavs = 0;
		const UINT offset = 0;
	public:
		D3D11RenderApi(int width, int height);
		~D3D11RenderApi();

		virtual void onStartUp() override;

		virtual void setIndexBuffer(IndexBuffer* buffer) override;
		virtual void setVertexBuffer(VertexBuffer* buffer) override;
		virtual void setTexture(UINT slot, Texture* texture) override;
		virtual void unsetTexture(UINT) override;
		virtual void setTextureColorBuffer(UINT slot, RenderTarget* renderTarget, UINT targetNum) override;
		virtual void setTextureDepthBuffer(UINT slot, RenderTarget* renderTarget) override;
		virtual void present() override;
		virtual void draw(UINT count) override;
		virtual void drawIndexed(UINT count) override;
		virtual void setGpuBuffer(GPUBuffer*) override;
		virtual void setViewport(int w0, int y0, int w, int y) override;
		virtual void setRenderTarget(RenderTarget* renderTarget) override;
		virtual void clearRenderTarget(RenderTarget* renderTarget, const float* color) override;
		virtual void setDepthTestFunction(DEPTH_TEST_FUNCTION func) override;
		virtual void setCullMode(CULL_MODE mode) override;
		virtual void setPipelineState(PipelineState* pipeline) override;
		virtual void setInputLayout(InputLayout* layout) override;

		void setSampler(int slot, D3D11SamplerState* sampler);
		void setRasterizerState(D3D11RasterizerState* rasterizer);
		void setBlendState(D3D11BlendState* blend, float* factor, unsigned int mask);

		D3D11Context& getContext() { return *context; }
	};
}

#endif
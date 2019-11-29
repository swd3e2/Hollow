#pragma once

#ifndef HW_D3D11_RENDER_API_H
#define HW_D3D11_RENDER_API_H

#include "Hollow/Graphics/RenderApi.h"
#include "D3D11Prerequisites.h"
#include "D3D11RenderStateManager.h"

namespace Hollow {
	class D3D11RenderApi : public RenderApi
	{
	private:
		D3D11Context* context;

		int width;
		int height;

		ID3D11RenderTargetView* m_RenderTarget;
		ID3D11DepthStencilView* m_DepthStencilView;

		ID3D11Texture2D* m_BackBuffer;
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11ShaderResourceView* m_DepthResourceView;

		D3D11_VIEWPORT vp;

		ID3D11ShaderResourceView* const pSRV[1] = { NULL };
		ID3D11UnorderedAccessView* pUAV[1] = { NULL };
		ID3D11RenderTargetView* nullRTV[3] = { NULL };

		const UINT uavs = 0;
		const UINT offset = 0;
	public:
		D3D11RenderApi(int width, int height);
		~D3D11RenderApi();

		virtual void onStartUp() override;

		virtual void setIndexBuffer(const s_ptr<IndexBuffer>& buffer) override;
		virtual void setVertexBuffer(const s_ptr<VertexBuffer>& buffer) override;
		virtual void setTexture(UINT slot, const s_ptr<Texture>& texture) override;
		virtual void unsetTexture(UINT) override;
		virtual void setTextureColorBuffer(UINT slot, const s_ptr<RenderTarget>& renderTarget, UINT targetNum) override;
		virtual void setTextureDepthBuffer(UINT slot, const s_ptr<RenderTarget>& renderTarget) override;
		virtual void present() override;
		virtual void draw(UINT count) override;
		virtual void drawIndexed(UINT count) override;
		virtual void setGpuBuffer(const s_ptr<GPUBuffer>& gBuffer) override;
		virtual void setViewport(int w0, int y0, int w, int y) override;
		virtual void setRenderTarget(const s_ptr<RenderTarget>& renderTarget) override;
		virtual void clearRenderTarget(const s_ptr<RenderTarget>& renderTarget, const float* color) override;
		virtual void setInputLayout(const s_ptr<InputLayout>& layout) override;

		virtual void setSampler(const int slot, const s_ptr<SamplerState>& sampler) override;
		virtual void setTextureSampler(const int textureUnit, const s_ptr<SamplerState>& sampler) override;
		virtual void setDepthStencilState(const s_ptr<DepthStencil>& depthStencil) override;
		virtual void setRasterizerState(const s_ptr<RasterizerState>& rasterizerState) override;
		virtual void setBlendState(const s_ptr<BlendState>& blendState) override;
		virtual void setShaderPipeline(const s_ptr<ShaderPipeline>& shaderPipeline) override;
		virtual void setPrimitiveTopology(const PrimitiveTopology topology) override;
		virtual void drawInstanced(UINT count, UINT instanceCount) override;
		virtual void drawIndexedInstanced(UINT count, UINT instanceCount) override;

		D3D11Context& getContext() { return *context; }
		
		virtual RendererType getRendererType() override { return RendererType::DirectX; }
	};
}

#endif
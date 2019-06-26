#pragma once

#ifndef HW_D3D11_RENDER_API_H
#define HW_D3D11_RENDER_API_H

#include "Hollow/Graphics/Renderer/Base/RenderApi.h"
#include "D3D11Prerequisites.h"

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
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11DepthStencilState* m_DepthStencilState;
		ID3D11ShaderResourceView* m_DepthResourceView;

		D3D11_VIEWPORT vp;

		D3D11TextureManager* textureManager;
		D3D11HardwareBufferManager* hardwareBufferManager;
		D3D11ShaderManager* shaderManager;
		D3D11BufferManager* gpuBufferManager;
		D3D11RenderTargetManager* renderTargetManager;

		ID3D11ShaderResourceView* const pSRV[1] = { NULL };
		ID3D11UnorderedAccessView* pUAV[1] = { NULL };

		D3D11RasterizerState* m_cullNone;
		D3D11RasterizerState* m_Wireframe;
		D3D11RasterizerState* m_cullBack;

		D3D11BlendState* blendState;
		const UINT uavs = 0;

		const UINT offset = 0;
		const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	public:
		D3D11RenderApi(int width, int height);
		~D3D11RenderApi();

		virtual void SetIndexBuffer(IndexBuffer* buffer) override;
		virtual void SetVertexBuffer(VertexBuffer* buffer) override;
		virtual void SetTexture(UINT slot, Texture* texture) override;
		virtual void UnsetTexture(UINT) override;
		virtual void SetTextureColorBuffer(UINT slot, RenderTarget* renderTarget) override;
		virtual void SetTextureDepthBuffer(UINT slot, RenderTarget* renderTarget) override;
		virtual void SetShader(ShaderProgram* shader) override;
		virtual void Present() override;
		virtual void Draw(UINT count) override;
		virtual void DrawIndexed(UINT count) override;
		virtual void SetGpuBuffer(GPUBuffer*) override;
		virtual void SetViewport(int w0, int y0, int w, int y) override;
		virtual void SetRenderTarget(RenderTarget* renderTarget) override;
		virtual void ClearRenderTarget(RenderTarget* renderTarget, const float* color) override;

		virtual void startUp() override;


		void FreeShaderResource(UINT slot);

		void SetContantBuffer(UINT slot, D3D11ConstantBuffer* cb);
		void SetSampler(int slot, D3D11SamplerState* sampler);
		void SetRasterizerState(D3D11RasterizerState* rasterizer);
		void SwapBuffers();
		void SetBlendState(D3D11BlendState* blend, float* factor, unsigned int mask);
		D3D11Context& getContext() { return *context; }
	};
}

#endif
#pragma once

#ifndef HW_D3D11_RENDER_API_H
#define HW_D3D11_RENDER_API_H

#include "Hollow/Graphics/Renderer/Base/RenderApi.h"
#include "D3D11Prerequisites.h"

class HOLLOW_API D3D11RenderApi : public RenderApi
{
public:
	bool vSync = true;
private:					
	D3D11Context*				context;

	int							width;
	int							height;

	D3D11SamplerState*			m_SamplerStateWrap;
	D3D11SamplerState*			m_SamplerStateClamp;

	D3D11TextureManager*		textureManager;
	D3D11HardwareBufferManager* hardwareBufferManager;
	D3D11ShaderManager*			shaderManager;
	D3D11WindowManager*			windowManager;

	ID3D11ShaderResourceView *const pSRV[1] = { NULL };
	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
private:
public:
	D3D11RenderApi(int width, int height);
	~D3D11RenderApi();
	
	virtual void SetIndexBuffer(IndexBuffer* buffer) override;
	virtual void SetVertexBuffer(VertexBuffer* buffer) override;
	virtual void SetTexture(UINT slot, Texture* texture) override;
	virtual void SetTexture(UINT slot, RenderTarget* renderTarget) override;
	virtual void SetShader(ShaderProgram* shader) override;

	virtual void startUp() override;

	void ClearRenderTargetView(D3D11RenderTarget* renderTarget, float* color);

	void FreeShaderResource(UINT slot);

	void SetVertexShader(D3D11VertexShader* vs);
	void SetPixelShader(D3D11PixelShader* ps);
	void ClearDepthStencilView(D3D11DepthStencil* ds, int flag);
	void SetContantBuffer(UINT slot, D3D11ConstantBuffer* cb);
	void SetSampler(int slot, D3D11SamplerState* sampler);
	void SetDepthStencil(D3D11DepthStencil* depthStencil);
	void SetRenderTarget(D3D11RenderTarget* renderTarget, D3D11DepthStencil* depthStencil);
	void SetViewPort(int numViewPorts, D3D11_VIEWPORT* vp);
	void SetRasterizerState(D3D11RasterizerState* rasterizer);
	void SwapBuffers();
	void SetBlendState(D3D11BlendState* blend, float* factor, unsigned int mask);
	void DrawIndexed(UINT count);
	void Draw(UINT count);
	void Present();
	D3D11Context& getContext() { return *context; }

	void toggleVSync() { vSync = !vSync; }
};

#endif
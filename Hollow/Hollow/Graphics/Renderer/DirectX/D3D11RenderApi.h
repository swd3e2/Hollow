#pragma once

#ifndef HW_D3D_RENDERER_H
#define HW_D3D_RENDERER_H

#include <d3d11.h>
#include "Hollow/Graphics/Renderer/Base/RenderApi.h"
#include "D3D11Prerequisites.h"

class D3D11RenderApi : public RenderApi
{
public:
	bool vSync = true;
private:					
	D3D11Context*				context;
	D3D11RasterizerState*		m_rasterizerState;
	int						width;
	int						height;

	ID3D11ShaderResourceView *const pSRV[1] = { NULL };
	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
private:
public:
	D3D11RenderApi(int width, int height, HWND* hwnd);
	~D3D11RenderApi();

	void SetShaderResource(UINT slot, ID3D11ShaderResourceView* shaderResourceView);
		
	void FreeShaderResource(UINT slot);

	void SetVertexShader(D3D11VertexShader* vs);
	void SetPixelShader(D3D11PixelShader* ps);
	void ClearDepthStencilView(D3D11DepthStencil* ds, int flag);
	void SetContantBuffer(UINT slot, D3D11ConstantBuffer* cb);
	void SetVertexBuffer(VertexBuffer* buffer);
	void ClearRenderTargetView(D3D11RenderTarget* renderTarget, float* color);
	void SetSampler(int slot, D3D11SamplerState* sampler);
	void SetDepthStencil(D3D11DepthStencil* depthStencil);
	void SetRenderTarget(D3D11RenderTarget* renderTarget, D3D11DepthStencil* depthStencil);
	void SetViewPort(int numViewPorts, D3D11_VIEWPORT* vp);
	void SetRasterizerState(D3D11RasterizerState* rasterizer);
	void SwapBuffers();
	void SetBlendState(D3D11BlendState* blend, float* factor, unsigned int mask);
	void DrawIndexed(UINT count);
	void Draw(UINT count);
	
	D3D11Context* getContext();

	void toggleVSync() { vSync = !vSync; }
};

#endif
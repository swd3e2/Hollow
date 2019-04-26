#pragma once

#ifndef HW_D3D_RENDERER_H
#define HW_D3D_RENDERER_H

#include <d3d11.h>
#include "Hollow/Graphics/Renderer/Base/IRenderApi.h"
#include "D3DPrerequisites.h"

class D3DRenderApi : public IRenderApi
{
public:
	bool vSync = true;
private:					
	D3DContext*				context;
	D3DRasterizerState*		m_rasterizerState;
	int						width;
	int						height;

	ID3D11ShaderResourceView *const pSRV[1] = { NULL };
	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
private:
public:
	D3DRenderApi(int width, int height, HWND* hwnd);
	~D3DRenderApi();

	void SetShaderResource(UINT slot, ID3D11ShaderResourceView* shaderResourceView);
		
	void FreeShaderResource(UINT slot);

	void SetVertexShader(D3DVertexShader* vs);
	void SetPixelShader(D3DPixelShader* ps);
	void ClearDepthStencilView(D3DDepthStencil* ds, int flag);
	void SetContantBuffer(UINT slot, D3DConstantBuffer* cb);
	void SetVertexBuffer(IVertexBuffer* buffer);
	void ClearRenderTargetView(D3DRenderTarget* renderTarget, float* color);
	void SetSampler(int slot, D3DSamplerState* sampler);
	void SetDepthStencil(D3DDepthStencil* depthStencil);
	void SetRenderTarget(D3DRenderTarget* renderTarget, D3DDepthStencil* depthStencil);
	void SetViewPort(int numViewPorts, D3D11_VIEWPORT* vp);
	void SetRasterizerState(D3DRasterizerState* rasterizer);
	void SwapBuffers();
	void SetBlendState(D3DBlendState* blend, float* factor, unsigned int mask);
	void DrawIndexed(UINT count);
	void Draw(UINT count);
	
	D3DContext* getContext();

	void toggleVSync() { vSync = !vSync; }
};

#endif
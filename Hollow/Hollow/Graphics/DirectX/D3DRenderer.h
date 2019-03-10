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
#include "Hollow/Graphics/IRenderer.h"
#include "D3DRenderable.h"
#include "D3DBlendState.h"
#include "D3DSamplerState.h"
#include "D3DRasterizerState.h"
#include "Hollow/Graphics/Window/Win32/Win32Window.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Platform.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Utils/FileSystem.h"
#include "D3DRenderable.h"
#include "Hollow/Resources/TextureLoader.h"
#include "Hollow/Graphics/TextureManager.h"
#include "D3DShaderManager.h"
#include "Hollow/Events/EventSystem.h"
#include "Hollow/Graphics/Events/BeginFrameEvent.h"
#include "Hollow/Graphics/Events/EndFrameEvent.h"
#include "Hollow/ECS/ComponentManager.h"
#include "Hollow/ECS/Components/PositionComponent.h"
#include "Hollow/ECS/Components/D3DRenderComponent.h"
#include "D3DConstBufferMapping.h"
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 600

using namespace DirectX;

struct WVP
{
	XMMATRIX WVP;
};

struct TransformBuff
{
	XMMATRIX transform;
};

struct Light
{
	Light() { direction = {}; ambient = {}; }
	XMFLOAT3 direction;
	float pad;
	XMFLOAT4 ambient;
};

// Simple directx renderer
class HOLLOW_API D3DRenderer : public IRenderer
{
public:
	Light												light;
private:
	Microsoft::WRL::ComPtr<ID3D11Device>				m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>				m_SwapChain;
	WVP													m_wvp;
	TransformBuff										transformBuff;
	D3DConstantBuffer*									m_LightBuffer;
	D3DConstantBuffer*									m_WVPConstantBuffer;
	D3DConstantBuffer*									m_TransformConstantBuffer;
	D3DBlendState*										m_BlendStateTransparancy;
	D3DSamplerState*									m_SamplerStateWrap;
	D3DSamplerState*									m_SamplerStateClamp;
	D3DRenderTarget*									m_RenderTarget;
	D3DDepthStencil*									m_DepthStencil;
	TextureManager*										textureManager;
	D3DShaderManager*									shaderManager;
	Win32Window											window;
	int width;
	int height;
	ID3D11ShaderResourceView *const pSRV[1] = { NULL };
	const UINT offset = 0;
	const float ClearColor[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
private:
public:
	D3DRenderer();
	~D3DRenderer();

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

	virtual void PreUpdateFrame() override;
	virtual void Update(std::vector<IRenderable*>* renderableList) override;
	void Draw(RenderableObject* vBuffer);
	virtual void PostUpdateFrame() override;
	virtual bool processMessage() override;

	virtual bool windowIsClosed() override { return window.isClosed(); }

	inline ID3D11Device* getDevice() { return m_Device.Get(); }
	inline ID3D11DeviceContext* getDeviceContext() { return m_DeviceContext.Get(); }
};

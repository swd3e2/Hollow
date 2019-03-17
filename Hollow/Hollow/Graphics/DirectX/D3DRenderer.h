#pragma once
#include <d3d11.h>
#include "Hollow/Common/Log.h"
#include "D3DRenderTarget.h"
#include "D3DDepthStencil.h"
#include "D3DPixelShader.h"
#include "D3DVertexShader.h"
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
#include "D3DRenderable.h"
#include "D3DConstBufferMapping.h"
#include "Icons/LightIcon.h"
#include "Hollow/Resources/ShaderManager.h"
#include "Hollow/Graphics/PointLight.h"
#include "Hollow/Graphics/DirectionalLight.h"
#include "D3DMaterial.h"

using namespace DirectX;

struct WVP
{
	XMMATRIX WVP;
};

struct WorldViewProjection
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
	XMFLOAT3 cameraPosition;
};

struct TransformBuff
{
	XMMATRIX transform;
};

// Simple directx renderer
class HOLLOW_API D3DRenderer : public IRenderer
{
public:
	PointLight*				pointLight;
private:					
	ID3D11Device*			m_Device;
	ID3D11DeviceContext*	m_DeviceContext;
	IDXGISwapChain*			m_SwapChain;
	WVP						m_wvp;
	WorldViewProjection		m_worldViewProjection;
	TransformBuff			transformBuff;
	// light sources
	DirectionalLight*		directionaltLight;
	// constant buffers
	D3DConstantBuffer*		m_LightBuffer;
	D3DConstantBuffer*		m_WVPConstantBuffer;
	D3DConstantBuffer*		m_WorldViewProjectionBuffer;
	D3DConstantBuffer*		m_TransformConstantBuffer;
	D3DConstantBuffer*		materialConstantBuffer;

	D3DBlendState*			m_BlendStateTransparancy;
	D3DSamplerState*		m_SamplerStateWrap;
	D3DSamplerState*		m_SamplerStateClamp;
	D3DRenderTarget*		m_RenderTarget;
	D3DDepthStencil*		m_DepthStencil;

	int pointLightsNum = 0;
	int directionalLightNum = 0;
	int spotLifhtNum = 0;

	bool vSync = true;

	Win32Window				window;
	int width;
	int height;
	ID3D11ShaderResourceView *const pSRV[1] = { NULL };
	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
private:
public:
	D3DRenderer(int width, int height);
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

	void DrawLight()
	{
		if (pointLight != nullptr) {
			
			XMVECTOR cameravector = m_Camera->GetPositionVector();
			XMVECTOR iconposvector = { 
				pointLight->lightIcon.renderable.transform->position.x, 
				pointLight->lightIcon.renderable.transform->position.y, 
				pointLight->lightIcon.renderable.transform->position.z, 
				1.0f
			};
			XMVECTOR icontocameravector = iconposvector - cameravector;

			pointLight->lightIcon.renderable.transform->rotation = m_Camera->GetRotationFloat3();

			D3DRenderable& dxRenderable = pointLight->lightIcon.renderable;

			transformBuff.transform = XMMatrixTranspose(
				(XMMatrixRotationRollPitchYaw(dxRenderable.transform->rotation.x, dxRenderable.transform->rotation.y, dxRenderable.transform->rotation.z) *
					XMMatrixScaling(dxRenderable.transform->scale.x, dxRenderable.transform->scale.y, dxRenderable.transform->scale.z)) *
				XMMatrixTranslation(dxRenderable.transform->position.x, dxRenderable.transform->position.y, dxRenderable.transform->position.z));

			m_TransformConstantBuffer->Update(&transformBuff);
			SetContantBuffer(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, m_TransformConstantBuffer);

			static float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			m_DeviceContext->OMSetBlendState(m_BlendStateTransparancy->GetBlendState(), blendFactor, 0xffffffff);

			for (RenderableObject* dxRenderableObject : dxRenderable.renderableObjects)
				Draw(dxRenderableObject);

			static float blendFactor1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			m_DeviceContext->OMSetBlendState(m_BlendStateTransparancy->GetBlendState(), blendFactor1, 0xffffffff);
		}
	}

	inline void DrawIndexed(UINT count) { m_DeviceContext->DrawIndexed(count, 0, 0); }
	inline void Draw(UINT count) { m_DeviceContext->Draw(count, 0); }

	virtual void PreUpdateFrame() override;
	virtual void Update(std::vector<IRenderable*>* renderableList) override;
	void Draw(RenderableObject* vBuffer);
	virtual void PostUpdateFrame() override;
	virtual bool processMessage() override;

	virtual bool windowIsClosed() override { return window.isClosed(); }

	inline ID3D11Device* getDevice() { return m_Device; }
	inline ID3D11DeviceContext* getDeviceContext() { return m_DeviceContext; }

	void setWindowIsClosed(bool status) { window.setIsClosed(status); }
	void toggleVSync() { vSync = !vSync; }
};

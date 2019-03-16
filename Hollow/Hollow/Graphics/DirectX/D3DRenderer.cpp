#include "D3DRenderer.h"

D3DRenderer::D3DRenderer(int width, int height) :
	window(GetModuleHandle(NULL), width, height)
{
	HRESULT hr = S_OK;
	RECT rc;

	GetClientRect(*window.getHWND(), &rc);
	this->width = rc.right - rc.left;
	this->height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.OutputWindow = *window.getHWND();
		
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_1;
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags,
		featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain,
		&m_Device, &m_featureLevel, &m_DeviceContext);

	if (hr != S_OK) {
		HW_ERROR("RenderSystem: Can't create DeviceAndSwapChain!");
	}

	D3D11_VIEWPORT	vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_DeviceContext->RSSetViewports(1, &vp);
	m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_RenderTarget = new D3DRenderTarget(m_Device, m_DeviceContext, m_SwapChain, width, height);
	m_DepthStencil = new D3DDepthStencil(m_Device, width, height, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);

	m_BlendStateTransparancy = new D3DBlendState(m_Device);

	m_SamplerStateWrap = new D3DSamplerState(m_Device, D3D11_TEXTURE_ADDRESS_WRAP);
	m_SamplerStateClamp = new D3DSamplerState(m_Device, D3D11_TEXTURE_ADDRESS_WRAP);

	m_WVPConstantBuffer = new D3DConstantBuffer(m_Device, m_DeviceContext, sizeof(WVP));
	m_TransformConstantBuffer = new D3DConstantBuffer(m_Device, m_DeviceContext, sizeof(TransformBuff));
	m_LightBuffer = new D3DConstantBuffer(m_Device, m_DeviceContext, sizeof(PointLightStruct));

	m_WorldViewProjectionBuffer = new D3DConstantBuffer(m_Device, m_DeviceContext, sizeof(WorldViewProjection));
	materialConstantBuffer = new D3DConstantBuffer(m_Device, m_DeviceContext, sizeof(MaterialData));
}

D3DRenderer::~D3DRenderer()
{
	SAFE_RELEASE(m_Device);
	SAFE_RELEASE(m_DeviceContext);
	SAFE_RELEASE(m_SwapChain);

	delete	m_WVPConstantBuffer;
	delete	m_TransformConstantBuffer;
	delete	m_BlendStateTransparancy;
	delete	m_SamplerStateWrap;
	delete	m_SamplerStateClamp;
	delete	m_RenderTarget;
	delete	m_DepthStencil;
}

void D3DRenderer::PreUpdateFrame()
{
	SetVertexShader(ShaderManager::instance()->getVertexShader("vs"));
	SetPixelShader(ShaderManager::instance()->getPixelShader("ps"));

	m_DeviceContext->ClearRenderTargetView(m_RenderTarget->GetMainRenderTaget(), ClearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_DeviceContext->PSSetSamplers(0, 1, m_SamplerStateWrap->GetSamplerState());
	m_DeviceContext->PSSetSamplers(1, 1, m_SamplerStateClamp->GetSamplerState());
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTarget->GetAddressOfMainRenderTaget(), m_DepthStencil->GetDepthStencilView());

	// Update world view projection matrix
	m_wvp.WVP = XMMatrixTranspose(XMMatrixIdentity()
		* m_Camera->GetViewMatrix()
		* m_Camera->GetProjectionMatrix());

	m_WVPConstantBuffer->Update(&m_wvp);
	SetContantBuffer(HOLLOW_CONST_BUFFER_WVP_SLOT, m_WVPConstantBuffer);

	m_worldViewProjection.World = XMMatrixIdentity();
	m_worldViewProjection.View = m_Camera->GetViewMatrix();
	m_worldViewProjection.Projection = m_Camera->GetProjectionMatrix();
	m_WorldViewProjectionBuffer->Update(&m_worldViewProjection);
	SetContantBuffer(HOLLOW_CONST_BUFFER_WOLRD_VIEW_PROJECTION_SLOT, m_WorldViewProjectionBuffer);
	
	if (pointLight != nullptr) {
		// update light
		m_LightBuffer->Update(&pointLight->data);
		SetContantBuffer(HOLLOW_CONST_BUFFER_LIGHT_SLOT, m_LightBuffer);
	}

	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_DeviceContext->OMSetBlendState(m_BlendStateTransparancy->GetBlendState(), blendFactor, 0xffffffff);
	//this->m_DeviceContext->GSSetShader(shaderManager->getGeometryShader("gs")->GetShader(), NULL, 0);
	m_worldViewProjection.cameraPosition = m_Camera->GetPositionFloat3();
}

void D3DRenderer::Draw(RenderableObject * object)
{
	if (object->material->diffuseTexture && object->material->diffuseTexture->active) {
		this->m_DeviceContext->PSSetShaderResources(0, 1, &object->material->diffuseTexture->m_TextureShaderResource);
	} else {
		this->m_DeviceContext->PSSetShaderResources(0, 1, pSRV);
	}
	if (object->material->normalTexture && object->material->normalTexture->active) {
		this->m_DeviceContext->PSSetShaderResources(1, 1, &object->material->normalTexture->m_TextureShaderResource);
	} else {
		this->m_DeviceContext->PSSetShaderResources(1, 1, pSRV);
	}

	materialConstantBuffer->Update(&object->material->materialData);
	SetContantBuffer(HOLLOW_CONST_BUFFER_MATERIAL_SLOT, materialConstantBuffer);

	this->m_DeviceContext->IASetVertexBuffers(0, 1, object->buffer->GetAddressOf(), object->buffer->StridePtr(), &this->offset);
	m_DeviceContext->Draw(object->buffer->BufferSize(), 0);
}

void D3DRenderer::PostUpdateFrame()
{
	DrawLight();
	m_SwapChain->Present(1, 0);
}

bool D3DRenderer::processMessage()
{
	return window.ProcessMessage();
}

void D3DRenderer::Update(std::vector<IRenderable*>* renderableList)
{
	for (auto& renderable : *renderableList)
	{
		D3DRenderable* dxRenderable = (D3DRenderable*)renderable;

		transformBuff.transform = XMMatrixTranspose(
			(XMMatrixTranslation(
				dxRenderable->transform->position.x, 
				dxRenderable->transform->position.y, 
				dxRenderable->transform->position.z) * 
			XMMatrixScaling(
				dxRenderable->transform->scale.x, 
				dxRenderable->transform->scale.y, 
				dxRenderable->transform->scale.z
			)) *
			XMMatrixRotationRollPitchYaw(
				dxRenderable->transform->rotation.x, 
				dxRenderable->transform->rotation.y, 
				dxRenderable->transform->rotation.z
			)
		);

		m_TransformConstantBuffer->Update(&transformBuff);
		SetContantBuffer(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, m_TransformConstantBuffer);

		for (RenderableObject* dxRenderableObject : dxRenderable->renderableObjects)
			Draw(dxRenderableObject);
	}
}

#pragma once
#include "DirectX/D3DRenderer.h"

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


struct LightMatrices
{
	XMMATRIX View;
	XMMATRIX Projection;
	XMFLOAT3 lightPosition;
};

struct TransformBuff
{
	XMMATRIX transform;
};

struct LightInfo
{
	PointLightStruct pointLights[10];
	int pointLightsNum = 1;
};

class ForwardRenderPass
{
public:
	PointLight*				pointLight;
	D3DRenderTarget*		m_SecondRenderTarget;
	ShadowMap*				shadowMap;
	Camera* m_Camera;
private:
	D3DRenderer* renderer;
private:
	WVP						m_wvp;
	WorldViewProjection		m_worldViewProjection;
	TransformBuff			transformBuff;
	D3DRasterizerState*		m_rasterizerState;
	// light sources
	DirectionalLight*		directionaltLight;
	LightMatrices			lightMatrices;

	D3DBlendState*			m_BlendStateTransparancy;
	D3DSamplerState*		m_SamplerStateWrap;
	D3DSamplerState*		m_SamplerStateClamp;
	D3DRenderTarget*		m_RenderTarget;
	D3DDepthStencil*		m_DepthStencil;
	D3DDepthStencil*		m_ShadowDepthStencil;

	// constant buffers
	D3DConstantBuffer*		m_LightBuffer;
	D3DConstantBuffer*		m_WVPConstantBuffer;
	D3DConstantBuffer*		m_WorldViewProjectionBuffer;
	D3DConstantBuffer*		m_TransformConstantBuffer;
	D3DConstantBuffer*		materialConstantBuffer;
	D3DConstantBuffer*		lightMatricesConstantBuffer;
	D3DConstantBuffer*		lightInfoBuffer;

	int pointLightsNum = 0;
	int directionalLightNum = 0;
	int spotLifhtNum = 0;
	
	D3D11_VIEWPORT vp;
	D3D11_VIEWPORT svp;

	ID3D11ShaderResourceView *const pSRV[1] = { NULL };
	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
public:
	ForwardRenderPass(D3DRenderer* renderer) : renderer(renderer)
	{
		m_RenderTarget = new D3DRenderTarget(renderer->getDevice(), renderer->getDeviceContext(), 1920, 1080, RenderTargetType::MAIN, DXGI_FORMAT_R32G32B32A32_FLOAT, renderer->getSwapChain());
		m_DepthStencil = new D3DDepthStencil(renderer->getDevice(), 1920, 1080, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);

		m_ShadowDepthStencil = new D3DDepthStencil(renderer->getDevice(), 1920, 1080, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);
		m_SecondRenderTarget = new D3DRenderTarget(renderer->getDevice(), renderer->getDeviceContext(), 1920, 1080, RenderTargetType::SECONDARY, DXGI_FORMAT_R32G32B32A32_FLOAT);

		m_BlendStateTransparancy = new D3DBlendState(renderer->getDevice());
		m_rasterizerState = new D3DRasterizerState(renderer->getDevice());

		m_SamplerStateWrap = new D3DSamplerState(renderer->getDevice(), D3D11_TEXTURE_ADDRESS_WRAP);
		m_SamplerStateClamp = new D3DSamplerState(renderer->getDevice(), D3D11_TEXTURE_ADDRESS_CLAMP);

		shadowMap = new ShadowMap(renderer->getDevice(), renderer->getDeviceContext(), 1920, 1080);

		m_ShadowDepthStencil = new D3DDepthStencil(renderer->getDevice(), 3840, 2160, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);

		m_WVPConstantBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(WVP));
		m_TransformConstantBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(TransformBuff));
		m_LightBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(PointLightStruct));
		lightMatricesConstantBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(LightMatrices));
		m_WorldViewProjectionBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(WorldViewProjection));
		materialConstantBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(MaterialData));
		lightInfoBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(LightInfo));

		shadowMap = new ShadowMap(renderer->getDevice(), renderer->getDeviceContext(), 3840, 2160);

		vp.Width = (float)1920;
		vp.Height = (float)1080;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		svp.Width = (float)3840;
		svp.Height = (float)2160;
		svp.MinDepth = 0.0f;
		svp.MaxDepth = 1.0f;
		svp.TopLeftX = 0;
		svp.TopLeftY = 0;

		renderer->getDeviceContext()->RSSetViewports(1, &vp);
	}

	void PreUpdateFrame()
	{
		renderer->getDeviceContext()->ClearRenderTargetView(m_RenderTarget->GetRenderTaget(), ClearColor);
		renderer->getDeviceContext()->ClearRenderTargetView(m_SecondRenderTarget->GetRenderTaget(), ClearColor);
		renderer->getDeviceContext()->ClearRenderTargetView(shadowMap->shadowRenderTarget.GetRenderTaget(), ClearColor);

		renderer->getDeviceContext()->ClearDepthStencilView(m_DepthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		renderer->getDeviceContext()->PSSetSamplers(0, 1, m_SamplerStateWrap->GetSamplerState());
		renderer->getDeviceContext()->PSSetSamplers(1, 1, m_SamplerStateClamp->GetSamplerState());

		// Setting states 
		renderer->getDeviceContext()->RSSetState(m_rasterizerState->GetRasterizerState());
		renderer->getDeviceContext()->OMSetDepthStencilState(m_DepthStencil->GetDepthStencilState(), 0);

		if (pointLight != nullptr) {
			// update light
			m_LightBuffer->Update(&pointLight->data);
			renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_LIGHT_SLOT, m_LightBuffer);
		}

		/*float blendFactor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_DeviceContext->OMSetBlendState(m_BlendStateTransparancy->GetBlendState(), blendFactor, 0xffffffff);*/
		//this->m_DeviceContext->GSSetShader(shaderManager->getGeometryShader("gs")->GetShader(), NULL, 0);
		m_worldViewProjection.cameraPosition = m_Camera->GetPositionFloat3();
	}

	void Update(std::vector<IRenderable*>* renderableList)
	{
		renderer->SetContantBuffer(5, lightMatricesConstantBuffer);

		drawShadowMap(renderableList);

		renderer->getDeviceContext()->OMSetRenderTargets(1, m_RenderTarget->GetAddressOfRenderTaget(), m_DepthStencil->GetDepthStencilView());
		renderer->getDeviceContext()->ClearDepthStencilView(m_DepthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		renderer->getDeviceContext()->PSSetShaderResources(3, 1, shadowMap->shadowRenderTarget.GetAddressOfShaderResourceView());

		updateWVP(m_Camera);

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
			renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, m_TransformConstantBuffer);

			for (RenderableObject* dxRenderableObject : dxRenderable->renderableObjects)
				DrawObject(dxRenderableObject);
		}

		this->renderer->getDeviceContext()->PSSetShaderResources(3, 1, pSRV);

		DrawLight();
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
			renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, m_TransformConstantBuffer);

			static float blendFactor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderer->getDeviceContext()->OMSetBlendState(m_BlendStateTransparancy->GetBlendState(), blendFactor, 0xffffffff);

			for (RenderableObject* dxRenderableObject : dxRenderable.renderableObjects)
				DrawObject(dxRenderableObject);

			renderer->getDeviceContext()->OMSetBlendState(0, 0, 0xffffffff);
		}
	}

	void drawShadowMap(std::vector<IRenderable*>* renderableList)
	{
		// Update light wvp matrix
		lightMatrices.Projection = XMMatrixTranspose(shadowMap->camera.GetProjectionMatrix());
		lightMatrices.View = XMMatrixTranspose(shadowMap->camera.GetViewMatrix());
		lightMatrices.lightPosition = shadowMap->camera.GetPositionFloat3();
		lightMatricesConstantBuffer->Update(&lightMatrices);

		// Set new viewport coz shadow map is bigger than vp
		renderer->getDeviceContext()->RSSetViewports(1, &svp);
		renderer->getDeviceContext()->ClearRenderTargetView(shadowMap->shadowRenderTarget.GetRenderTaget(), ShadowClearColor);
		// Shadow map drawing
		renderer->getDeviceContext()->ClearDepthStencilView(m_ShadowDepthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		renderer->getDeviceContext()->OMSetRenderTargets(1, shadowMap->shadowRenderTarget.GetAddressOfRenderTaget(), m_ShadowDepthStencil->GetDepthStencilView());

		updateWVP(&shadowMap->camera);


		renderer->SetVertexShader(ShaderManager::instance()->getVertexShader("depthVS"));
		renderer->SetPixelShader(ShaderManager::instance()->getPixelShader("depthPS"));
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
			renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, m_TransformConstantBuffer);

			for (RenderableObject* dxRenderableObject : dxRenderable->renderableObjects)
			{
				renderer->getDeviceContext()->IASetVertexBuffers(0, 1, dxRenderableObject->buffer->GetAddressOf(), dxRenderableObject->buffer->StridePtr(), &this->offset);
				renderer->getDeviceContext()->Draw(dxRenderableObject->buffer->BufferSize(), 0);
			}
		}
		// Set viewprort to default
		renderer->getDeviceContext()->RSSetViewports(1, &vp);
	}

	// Update world view projection matrix
	void updateWVP(Camera* camera)
	{
		m_wvp.WVP = XMMatrixTranspose(
			XMMatrixIdentity()
			* camera->GetViewMatrix()
			* camera->GetProjectionMatrix()
		);

		m_WVPConstantBuffer->Update(&m_wvp);
		renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_WVP_SLOT, m_WVPConstantBuffer);

		m_worldViewProjection.World = XMMatrixIdentity();
		m_worldViewProjection.View = XMMatrixTranspose(camera->GetViewMatrix());
		m_worldViewProjection.Projection = XMMatrixTranspose(camera->GetProjectionMatrix());
		m_WorldViewProjectionBuffer->Update(&m_worldViewProjection);
		renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_WOLRD_VIEW_PROJECTION_SLOT, m_WorldViewProjectionBuffer);
	}

	void DrawObject(RenderableObject * object)
	{
		if (object->material->diffuseTexture && object->material->diffuseTexture->active) {
			renderer->getDeviceContext()->PSSetShaderResources(0, 1, &object->material->diffuseTexture->m_TextureShaderResource);
		}
		else {
			renderer->getDeviceContext()->PSSetShaderResources(0, 1, pSRV);
		}
		if (object->material->normalTexture && object->material->normalTexture->active) {
			renderer->getDeviceContext()->PSSetShaderResources(1, 1, &object->material->normalTexture->m_TextureShaderResource);
		}
		else {
			renderer->getDeviceContext()->PSSetShaderResources(1, 1, pSRV);
		}
		if (object->material->specularTexture && object->material->specularTexture->active) {
			renderer->getDeviceContext()->PSSetShaderResources(2, 1, &object->material->specularTexture->m_TextureShaderResource);
		}
		else {
			renderer->getDeviceContext()->PSSetShaderResources(2, 1, pSRV);
		}

		materialConstantBuffer->Update(&object->material->materialData);
		renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_MATERIAL_SLOT, materialConstantBuffer);

		renderer->SetVertexShader(object->material->vertexShader);
		renderer->SetPixelShader(object->material->pixelShader);

		renderer->getDeviceContext()->IASetVertexBuffers(0, 1, object->buffer->GetAddressOf(), object->buffer->StridePtr(), &this->offset);
		renderer->getDeviceContext()->Draw(object->buffer->BufferSize(), 0);
	}

	void PostUpdateFrame()
	{
		renderer->getSwapChain()->Present(renderer->vSync, 0);
	}
};
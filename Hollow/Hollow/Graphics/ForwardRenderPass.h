#pragma once
#include "DirectX/D3DRenderer.h"
#include "Hollow/ECS/RenderableComponent.h"
#include "Hollow/ECS/TransformComponent.h"
#include "Hollow/ECS/System.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/GameObject.h"
#include "Hollow/ECS/Light.h"
#include "Hollow/ECS/PointLightComponent.h"

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
	float bias;
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

class ForwardRenderPass : public Hollow::System<ForwardRenderPass>
{
public:
	LightInfo				pointLights;
	PointLight*				pointLight;
	D3DRenderTarget*		m_SecondRenderTarget;
	ShadowMap*				shadowMap;
	Camera*					m_Camera;
	D3DDepthStencil*		m_ShadowDepthStencil;
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

		m_ShadowDepthStencil = new D3DDepthStencil(renderer->getDevice(), 8192, 8192, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);

		m_WVPConstantBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(WVP));
		m_TransformConstantBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(TransformBuff));
		m_LightBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(PointLightStruct));
		lightMatricesConstantBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(LightMatrices));
		m_WorldViewProjectionBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(WorldViewProjection));
		materialConstantBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(MaterialData));
		lightInfoBuffer = new D3DConstantBuffer(renderer->getDevice(), renderer->getDeviceContext(), sizeof(LightInfo));

		shadowMap = new ShadowMap(renderer->getDevice(), renderer->getDeviceContext(), 8192, 8192);

		vp.Width = (float)1920;
		vp.Height = (float)1080;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		svp.Width = (float)8192;
		svp.Height = (float)8192;
		svp.MinDepth = 0.0f;
		svp.MaxDepth = 1.0f;
		svp.TopLeftX = 0;
		svp.TopLeftY = 0;

		renderer->getDeviceContext()->RSSetViewports(1, &vp);
	}

	virtual void PreUpdate(float_t dt)
	{
		renderer->ClearRenderTargetView(m_RenderTarget, (float*)ClearColor);
		renderer->ClearRenderTargetView(&shadowMap->shadowRenderTarget, (float*)ClearColor);

		renderer->ClearDepthStencilView(m_DepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL);

		renderer->SetSampler(0, m_SamplerStateClamp);
		renderer->SetSampler(1, m_SamplerStateWrap);

		// Setting states 
		renderer->getDeviceContext()->RSSetState(m_rasterizerState->GetRasterizerState());
		renderer->SetDepthStencil(m_DepthStencil);

		int j = 0;
		for (auto& it : EntityManager::instance()->getContainer<Light>()->entityList)
		{
			if (it.hasComponent<PointLightComponent>() && it.getComponent<TransformComponent>()) {
				TransformComponent* transform = it.getComponent<TransformComponent>();
				PointLightComponent* light = it.getComponent<PointLightComponent>();
				
				pointLights.pointLights[j] = light->light.data;

				pointLights.pointLights[j].position[0] = transform->position.x;
				pointLights.pointLights[j].position[1] = transform->position.y;
				pointLights.pointLights[j].position[2] = transform->position.z;
				j++;
			}
		}
		lightInfoBuffer->Update(&pointLights);
		renderer->SetContantBuffer(6, lightInfoBuffer);

		/*float blendFactor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_DeviceContext->OMSetBlendState(m_BlendStateTransparancy->GetBlendState(), blendFactor, 0xffffffff);*/
		//this->m_DeviceContext->GSSetShader(shaderManager->getGeometryShader("gs")->GetShader(), NULL, 0);

		// todo: wtf
		m_worldViewProjection.cameraPosition = m_Camera->GetPositionFloat3();
	}

	virtual void Update(float_t dt)
	{
		DrawShadowMap();
		DrawScene();
		DrawLight();
	}

	virtual void PostUpdate(float_t dt)
	{
		renderer->getSwapChain()->Present(renderer->vSync, 0);
	}

	void DrawScene()
	{
		renderer->SetRenderTarget(m_RenderTarget, m_DepthStencil);
		renderer->ClearDepthStencilView(m_DepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL);

		renderer->SetShaderResource(3, *m_ShadowDepthStencil->GetAddressOfDepthStencilResource());

		updateWVP(m_Camera);

		for (auto& entity : EntityManager::instance()->getContainer<GameObject>()->entityList)
		{
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<RenderableComponent>())
			{
				TransformComponent* transform = entity.getComponent<TransformComponent>();
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();

				transformBuff.transform = XMMatrixTranspose(
					(XMMatrixTranslation(
						transform->position.x,
						transform->position.y,
						transform->position.z) *
						XMMatrixScaling(
							transform->scale.x,
							transform->scale.y,
							transform->scale.z
						)) *
					XMMatrixRotationRollPitchYaw(
						transform->rotation.x,
						transform->rotation.y,
						transform->rotation.z
					)
				);

				m_TransformConstantBuffer->Update(&transformBuff);
				renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, m_TransformConstantBuffer);

				for (RenderableObject* dxRenderableObject : renderable->renderable.renderableObjects)
					DrawObject(dxRenderableObject);
			}
		}

		this->renderer->SetShaderResource(3, *pSRV);
	}

	/*
	 * Draw light icon in light position
	 */
	void DrawLight()
	{
		for (auto& it : EntityManager::instance()->getContainer<Light>()->entityList)
		{
			if (it.hasComponent<PointLightComponent>() && it.hasComponent<TransformComponent>()) {
				PointLightComponent* light = it.getComponent<PointLightComponent>();
				TransformComponent* transform = it.getComponent<TransformComponent>();
				transform->rotation = m_Camera->GetRotationFloat3();

				transformBuff.transform = XMMatrixTranspose(
					(XMMatrixRotationRollPitchYaw(transform->rotation.x, transform->rotation.y, transform->rotation.z) *
						XMMatrixScaling(transform->scale.x, transform->scale.y, transform->scale.z)) *
					XMMatrixTranslation(transform->position.x, transform->position.y, transform->position.z));

				m_TransformConstantBuffer->Update(&transformBuff);
				renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, m_TransformConstantBuffer);

				static float blendFactor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
				renderer->getDeviceContext()->OMSetBlendState(m_BlendStateTransparancy->GetBlendState(), blendFactor, 0xffffffff);

				renderer->SetPixelShader(ShaderManager::instance()->getPixelShader("light"));
				DrawObject(light->light.lightIcon.renderable.renderableObjects[0]);
				
				renderer->getDeviceContext()->OMSetBlendState(0, 0, 0xffffffff);
			}
		}
	}

	// draw shadow map for single light source
	void DrawShadowMap()
	{
		// Update light wvp matrix
		lightMatrices.Projection = XMMatrixTranspose(shadowMap->camera.GetProjectionMatrix());
		lightMatrices.View = XMMatrixTranspose(shadowMap->camera.GetViewMatrix());
		lightMatrices.lightPosition = shadowMap->camera.GetPositionFloat3();
		lightMatrices.bias = shadowMap->bias;
		lightMatricesConstantBuffer->Update(&lightMatrices);
		renderer->SetContantBuffer(5, lightMatricesConstantBuffer);

		// Set new viewport coz shadow map is bigger than vp
		renderer->getDeviceContext()->RSSetViewports(1, &svp);
		renderer->ClearRenderTargetView(&shadowMap->shadowRenderTarget, (float*)ShadowClearColor);
		// Shadow map drawing
		renderer->getDeviceContext()->ClearDepthStencilView(m_ShadowDepthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		renderer->SetRenderTarget(&shadowMap->shadowRenderTarget, m_ShadowDepthStencil);

		updateWVP(&shadowMap->camera);

		renderer->SetVertexShader(ShaderManager::instance()->getVertexShader("depthVS"));
		renderer->SetPixelShader(ShaderManager::instance()->getPixelShader("depthPS"));

		for (auto& entity : EntityManager::instance()->getContainer<GameObject>()->entityList)
		{
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<RenderableComponent>())
			{
				TransformComponent* transform = entity.getComponent<TransformComponent>();
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();

				transformBuff.transform = XMMatrixTranspose(
					(XMMatrixTranslation(
						transform->position.x,
						transform->position.y,
						transform->position.z) *
						XMMatrixScaling(
							transform->scale.x,
							transform->scale.y,
							transform->scale.z
						)) *
					XMMatrixRotationRollPitchYaw(
						transform->rotation.x,
						transform->rotation.y,
						transform->rotation.z
					)
				);

				m_TransformConstantBuffer->Update(&transformBuff);
				renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, m_TransformConstantBuffer);

				for (RenderableObject* dxRenderableObject : renderable->renderable.renderableObjects)
				{
					renderer->SetVertexBuffer(dxRenderableObject->buffer);
					renderer->Draw(dxRenderableObject->buffer->BufferSize());
				}
			}
		}
		// Set viewprort to default
		renderer->getDeviceContext()->RSSetViewports(1, &vp);
	}

	// Update world view projection matrix
	void updateWVP(Camera* camera)
	{
		m_wvp.WVP = XMMatrixTranspose(
			camera->GetViewMatrix()
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
			renderer->SetShaderResource(0, object->material->diffuseTexture->m_TextureShaderResource);
		}
		else {
			renderer->FreeShaderResource(0);
		}
		if (object->material->normalTexture && object->material->normalTexture->active) {
			renderer->SetShaderResource(1, object->material->normalTexture->m_TextureShaderResource);
		}
		else {
			renderer->FreeShaderResource(1);
		}
		if (object->material->specularTexture && object->material->specularTexture->active) {
			renderer->SetShaderResource(2, object->material->specularTexture->m_TextureShaderResource);
		}
		else {
			renderer->FreeShaderResource(2);
		}

		materialConstantBuffer->Update(&object->material->materialData);
		renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_MATERIAL_SLOT, materialConstantBuffer);

		renderer->SetVertexShader(object->material->vertexShader);
		renderer->SetPixelShader(object->material->pixelShader);

		renderer->SetVertexBuffer(object->buffer);
		renderer->Draw(object->buffer->BufferSize());
	}
};
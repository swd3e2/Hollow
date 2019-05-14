#pragma once
#include "Renderer/DirectX/D3D11RenderApi.h"
#include "Hollow/ECS/RenderableComponent.h"
#include "Hollow/ECS/TransformComponent.h"
#include "Hollow/ECS/System.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/GameObject.h"
#include "Hollow/ECS/Light.h"
#include "Hollow/ECS/PointLightComponent.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Graphics/Camera.h"
#include "Renderer/DirectX/D3D11DepthStencil.h"
#include "Renderer/DirectX/D3D11RenderTarget.h"
#include "ShadowMap.h"
#include "Renderer/DirectX/D3D11BlendState.h"
#include "Renderer/DirectX/D3D11RasterizerState.h"
#include "Renderer/DirectX/D3D11ConstantBuffer.h"
#include "Renderer/DirectX/D3D11ConstBufferMapping.h"
#include "Renderer/DirectX/D3D11ShaderResourceMapping.h"
#include "Hollow/Resources/Mesh/Mesh.h"
#include "Renderer/DirectX/D3D11Context.h"
#include "ShaderManager.h"
#include "SkyMap.h"
#include "Water.h"
#include "Renderer/DirectX/D3D11ShaderManager.h"
#include "Renderer/DirectX/D3D11Texture.h"
#include "Hollow/Input/InputManager.h"

using namespace DirectX;

struct WVP
{
	Matrix4 WVP;
	float gMaxTessDistance;
	float gMinTessDistance;
	float gMinTessFactor;
	float gMaxTessFactor;
};

struct WorldViewProjection
{
	Matrix4 World;
	Matrix4 View;
	Matrix4 Projection;
	Vector3 cameraPosition;
	float offset;
};


struct LightMatrices
{
	Matrix4 View;
	Matrix4 Projection;
	Vector3 lightPosition;
	float bias;
};

struct TransformBuff
{
	Matrix4 transform;
};

struct LightInfo
{
	PointLightStruct pointLights[10];
	int pointLightsNum = 1;
};

class ForwardRenderSystem : public Hollow::System<ForwardRenderSystem>
{
public:
	Water*					water;
	LightInfo				pointLights;
	PointLight*				pointLight;
	D3D11RenderTarget*		m_SecondRenderTarget;
	ShadowMap*				shadowMap;
	Camera*					m_Camera;
	D3D11DepthStencil*		m_ShadowDepthStencil;
	SkyMap*					skyMap;
private:
	D3D11RenderApi* renderer;
private:
	WVP						m_wvp;
	WorldViewProjection		m_worldViewProjection;
	TransformBuff			transformBuff;
	D3D11RasterizerState* m_cullNone;
	D3D11RasterizerState* m_Wireframe;
	D3D11RasterizerState* m_cullBack;

	// light sources
	LightMatrices			lightMatrices;

	D3D11BlendState*		m_BlendStateTransparancy;
	D3D11SamplerState*		m_SamplerStateWrap;
	D3D11SamplerState*		m_SamplerStateClamp;
	D3D11RenderTarget*		m_RenderTarget;
	D3D11DepthStencil*		m_DepthStencil;

	// constant buffers
	D3D11ConstantBuffer*	m_LightBuffer;
	D3D11ConstantBuffer*	m_WVPConstantBuffer;
	D3D11ConstantBuffer*	m_WorldViewProjectionBuffer;
	D3D11ConstantBuffer*	m_TransformConstantBuffer;
	D3D11ConstantBuffer*	materialConstantBuffer;
	D3D11ConstantBuffer*	lightMatricesConstantBuffer;
	D3D11ConstantBuffer*	lightInfoBuffer;
	D3D11ConstantBuffer*	boneInfo;

	int pointLightsNum = 0;
	int directionalLightNum = 0;
	int spotLifhtNum = 0;

	D3D11_VIEWPORT vp;
	D3D11_VIEWPORT svp;

	const UINT uavs = 0;

	ID3D11UnorderedAccessView* const uav[1] = { NULL };

	int rasterizer = 0;

	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
public:
	ForwardRenderSystem(D3D11RenderApi* renderer) : renderer(renderer)
	{
		m_RenderTarget = new D3D11RenderTarget(2560, 1440, RenderTargetType::MAIN, DXGI_FORMAT_R32G32B32A32_FLOAT);
		m_DepthStencil = new D3D11DepthStencil(2560, 1440, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);

		m_ShadowDepthStencil = new D3D11DepthStencil(2560, 1440, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);
		m_SecondRenderTarget = new D3D11RenderTarget(2560, 1440, RenderTargetType::SECONDARY, DXGI_FORMAT_R32G32B32A32_FLOAT);

		m_BlendStateTransparancy = new D3D11BlendState();

		m_cullNone = new D3D11RasterizerState(D3D11_CULL_MODE::D3D11_CULL_NONE, D3D11_FILL_MODE::D3D11_FILL_SOLID);
		m_Wireframe = new D3D11RasterizerState(D3D11_CULL_MODE::D3D11_CULL_NONE, D3D11_FILL_MODE::D3D11_FILL_WIREFRAME);
		m_cullBack = new D3D11RasterizerState(D3D11_CULL_MODE::D3D11_CULL_BACK, D3D11_FILL_MODE::D3D11_FILL_SOLID);

		m_ShadowDepthStencil = new D3D11DepthStencil(8192, 8192, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);

		m_WVPConstantBuffer = new D3D11ConstantBuffer(sizeof(WVP));
		m_TransformConstantBuffer = new D3D11ConstantBuffer(sizeof(TransformBuff));
		m_LightBuffer = new D3D11ConstantBuffer(sizeof(PointLightStruct));
		lightMatricesConstantBuffer = new D3D11ConstantBuffer(sizeof(LightMatrices));
		m_WorldViewProjectionBuffer = new D3D11ConstantBuffer(sizeof(WorldViewProjection));
		materialConstantBuffer = new D3D11ConstantBuffer(sizeof(MaterialData));
		lightInfoBuffer = new D3D11ConstantBuffer(sizeof(LightInfo));
		boneInfo = new D3D11ConstantBuffer(sizeof(Matrix4) * 100);

		shadowMap = new ShadowMap(8192, 8192);

		vp.Width = (float)2560;
		vp.Height = (float)1440;
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

		renderer->SetViewPort(1, &vp);

		m_worldViewProjection.offset = 0.0f;
		renderer->SetRasterizerState(m_cullNone);
	}

	virtual void PreUpdate(float_t dt)
	{
		if (InputManager::instance()->GetKeyboardKeyIsPressed(eKeyCodes::KEY_F1))
		{
			renderer->SetRasterizerState(m_cullBack);
		}
		else if (InputManager::instance()->GetKeyboardKeyIsPressed(eKeyCodes::KEY_F2))
		{
			renderer->SetRasterizerState(m_cullNone);
		}
		else if (InputManager::instance()->GetKeyboardKeyIsPressed(eKeyCodes::KEY_F3))
		{
			renderer->SetRasterizerState(m_Wireframe);
		}
		renderer->ClearRenderTargetView(m_RenderTarget, (float*)ClearColor);
		renderer->ClearRenderTargetView(&shadowMap->shadowRenderTarget, (float*)ClearColor);

		renderer->ClearDepthStencilView(m_DepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL);

		// Setting states 
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

		m_worldViewProjection.cameraPosition = m_Camera->GetPositionVec3();

		renderer->SetRenderTarget(m_RenderTarget, m_DepthStencil);
	}

	virtual void Update(float_t dt)
	{
		m_worldViewProjection.offset += dt * 0.1f;
		m_worldViewProjection.offset = fmod(m_worldViewProjection.offset, 139.0f);
		DrawSkyMap();
		updateWVP(m_Camera);

		DrawWater();
		DrawScene();
	}

	virtual void PostUpdate(float_t dt)
	{
		renderer->Present();
	}

	void DrawScene()
	{
		for (auto& entity : EntityManager::instance()->getContainer<GameObject>()->entityList)
		{
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<RenderableComponent>())
			{
				TransformComponent* transform = entity.getComponent<TransformComponent>();
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();

				if (renderable->mesh->getReady())
				{
					transformBuff.transform = Matrix4::Transpose(
						Matrix4::Scaling(transform->scale.x, transform->scale.y, transform->scale.z) *
						Matrix4::Rotation(transform->rotation.x, transform->rotation.y, transform->rotation.z)* 
						Matrix4::Translation(33, 22, transform->position.z)
					);
					m_TransformConstantBuffer->Update(&transformBuff);
					renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, m_TransformConstantBuffer);

					if (renderable->mesh->hasAnimation())
					{
						renderable->mesh->animate(m_worldViewProjection.offset, renderable->mesh->rootBone, Matrix4::Identity());
						boneInfo->Update(renderable->mesh->boneInfo);
						renderer->SetContantBuffer(7, boneInfo);
					}

					for (auto& subMesh : renderable->mesh->subMeshes)
					{
						DrawObject(subMesh);
					}
				}
			}
		}
	}

	// Update world view projection matrix
	void updateWVP(Camera* camera)
	{
		m_wvp.WVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();

		m_WVPConstantBuffer->Update(&m_wvp);
		renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_WVP_SLOT, m_WVPConstantBuffer);

		m_worldViewProjection.World = Matrix4::Identity();
		m_worldViewProjection.View = camera->GetViewMatrix();
		m_worldViewProjection.Projection = camera->GetProjectionMatrix();
		m_WorldViewProjectionBuffer->Update(&m_worldViewProjection);
		renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_WOLRD_VIEW_PROJECTION_SLOT, m_WorldViewProjectionBuffer);
	}

	void DrawObject(SubMesh * object)
	{
		if (object->material != nullptr)
		{
			if (object->material->diffuse_texture != nullptr) {
				renderer->SetTexture(0, object->material->diffuse_texture);
			}
			else {
				renderer->FreeShaderResource(0);
			}
			if (object->material->normal_texture != nullptr) {
				renderer->SetTexture(1, object->material->normal_texture);
			}
			else {
				renderer->FreeShaderResource(1);
			}
			if (object->material->specular_texture != nullptr) {
				renderer->SetTexture(2, object->material->specular_texture);
			}
			else {
				renderer->FreeShaderResource(2);
			}

			materialConstantBuffer->Update(&object->material->materialData);
			renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_MATERIAL_SLOT, materialConstantBuffer);

			renderer->SetShader(object->material->shader);
		}
		else {
			renderer->SetShader(ShaderManager::instance()->getShader("default"));
		}
		renderer->SetVertexBuffer(object->vBuffer);
		renderer->SetIndexBuffer(object->iBuffer);
		renderer->DrawIndexed(object->iBuffer->getSize());
	}

	void DrawSkyMap()
	{
		Matrix4 viewMatrx = m_Camera->GetViewMatrix();
		viewMatrx.md[0][3] = 0.0f;
		viewMatrx.md[1][3] = 0.0f;
		viewMatrx.md[2][3] = 0.0f;
		m_wvp.WVP = m_Camera->GetProjectionMatrix() * viewMatrx;

		m_WVPConstantBuffer->Update(&m_wvp);
		renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_WVP_SLOT, m_WVPConstantBuffer);
		DrawObject(skyMap->mesh->subMeshes[0]);
	}

	void DrawWater()
	{
		D3D11Context& context = renderer->getContext();
		/*context.getDeviceContext()->CSSetShader(static_cast<D3D11ComputeShader*>(water->computeShader)->GetShader(), NULL, 0);
		
		context.getDeviceContext()->CSSetUnorderedAccessViews(0, 1, &static_cast<D3D11Texture*>(water->tex)->m_UnorderedAccessView, &uavs);
		context.getDeviceContext()->Dispatch(16, 16, 1);
		context.getDeviceContext()->CSSetUnorderedAccessViews(0, 1, uav, &uavs);*/

		context.getDeviceContext()->PSSetShaderResources(5, 1, &static_cast<D3D11Texture*>(water->tex)->m_TextureShaderResource);

		/*context.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
		context.getDeviceContext()->HSSetShader(static_cast<D3D11HullShader*>(water->mesh->subMeshes[0]->material->shader->getHullShader())->GetShader(), NULL, 0);
		context.getDeviceContext()->DSSetShader(static_cast<D3D11DomainShader*>(water->mesh->subMeshes[0]->material->shader->getDomainShader())->GetShader(), NULL, 0);
		context.getDeviceContext()->GSSetShader(NULL, NULL, 0);*/

		DrawObject(water->mesh->subMeshes[0]);

		renderer->FreeShaderResource(5);

		context.getDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context.getDeviceContext()->HSSetShader(NULL, NULL, 0);
		context.getDeviceContext()->DSSetShader(NULL, NULL, 0);
	}
};
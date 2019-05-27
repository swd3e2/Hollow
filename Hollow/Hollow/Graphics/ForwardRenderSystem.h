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
#include "Hollow/Resources/Mesh/Mesh.h"
#include "ShaderManager.h"
#include "SkyMap.h"
#include "Water.h"
#include "Hollow/Input/InputManager.h"
#include "Hollow/ECS/AnimationComponent.h"
#include "GPUBufferManager.h"

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
	bool hasAnimation;
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
	Camera*					m_Camera;
	SkyMap*					skyMap;
private:
	RenderApi* renderer;
private:
	WVP						m_wvp;
	WorldViewProjection		m_worldViewProjection;
	TransformBuff			transformBuff;
	LightMatrices			lightMatrices;
	
	GPUBuffer*	m_WVPConstantBuffer;
	GPUBuffer*	m_WorldViewProjectionBuffer;
	GPUBuffer*	m_TransformConstantBuffer;
	GPUBuffer*	materialConstantBuffer;
	GPUBuffer*	lightInfoBuffer;
	GPUBuffer*	boneInfo;

	int pointLightsNum = 0;
	int directionalLightNum = 0;
	int spotLifhtNum = 0;

	D3D11_VIEWPORT vp;

	const UINT uavs = 0;

	int rasterizer = 0;

	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
public:
	ForwardRenderSystem(RenderApi* renderer) : renderer(renderer)
	{
		m_WVPConstantBuffer			= GPUBufferManager::instance()->create(0, sizeof(WVP));
		m_WorldViewProjectionBuffer	= GPUBufferManager::instance()->create(1, sizeof(WorldViewProjection));
		m_TransformConstantBuffer	= GPUBufferManager::instance()->create(2, sizeof(TransformBuff));
		materialConstantBuffer		= GPUBufferManager::instance()->create(4, sizeof(MaterialData));
		lightInfoBuffer				= GPUBufferManager::instance()->create(6, sizeof(LightInfo));
		boneInfo					= GPUBufferManager::instance()->create(7, sizeof(Matrix4) * 100);

		renderer->SetViewport(0, 0, 1366, 768);
		m_worldViewProjection.offset = 0.0f;
	}

	virtual void PreUpdate(double dt)
	{
		renderer->ClearRenderTarget(0, (float*)ClearColor);
		renderer->SetRenderTarget(0);

		int j = 0;
		for (auto& it : EntityManager::instance()->getContainer<Light>()->entityList) {
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
		lightInfoBuffer->update(&pointLights);
		renderer->SetGpuBuffer(lightInfoBuffer);

		m_worldViewProjection.cameraPosition = m_Camera->GetPositionVec3();
	}

	virtual void Update(double dt)
	{
		updateWVP(m_Camera);
		//DrawWater();
		DrawScene();
		DrawSkyMap();
	}

	virtual void PostUpdate(double dt)
	{
		renderer->Present();
	}

	void DrawScene()
	{
		for (auto& entity : EntityManager::instance()->getContainer<GameObject>()->entityList) {
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<RenderableComponent>()) {
				TransformComponent* transform = entity.getComponent<TransformComponent>();
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();

				transformBuff.transform = Matrix4::Transpose(
					Matrix4::Scaling(transform->scale.x, transform->scale.y, transform->scale.z) *
					Matrix4::Rotation(transform->rotation.x, transform->rotation.y, transform->rotation.z)* 
					Matrix4::Translation(33, 22, transform->position.z)
				);
					
				if (entity.hasComponent<AnimationComponent>()) {
					AnimationComponent* animationComponent = entity.getComponent<AnimationComponent>();
					transformBuff.hasAnimation = true;
					boneInfo->update(animationComponent->boneInfo);
					renderer->SetGpuBuffer(boneInfo);
				} else {
					transformBuff.hasAnimation = false;
				}

				m_TransformConstantBuffer->update(&transformBuff);
				renderer->SetGpuBuffer(m_TransformConstantBuffer);

				for (auto& model : renderable->mesh->models) {
					DrawObject(model);
				}
			}
		}
	}

	// Update world view projection matrix
	void updateWVP(Camera* camera)
	{
		m_wvp.WVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();

		m_WVPConstantBuffer->update(&m_wvp);
		renderer->SetGpuBuffer(m_WVPConstantBuffer);

		m_worldViewProjection.World = Matrix4::Identity();
		m_worldViewProjection.View = camera->GetViewMatrix();
		m_worldViewProjection.Projection = camera->GetProjectionMatrix();
		m_WorldViewProjectionBuffer->update(&m_worldViewProjection);

		renderer->SetGpuBuffer(m_WorldViewProjectionBuffer);
	}

	void DrawObject(Model * object)
	{
		if (object->material != nullptr) {
			if (object->material->diffuse_texture != nullptr) {
				renderer->SetTexture(0, object->material->diffuse_texture);
			}
			if (object->material->normal_texture != nullptr) {
				renderer->SetTexture(1, object->material->normal_texture);
			}
			if (object->material->specular_texture != nullptr) {
				renderer->SetTexture(2, object->material->specular_texture);
			}

			materialConstantBuffer->update(&object->material->materialData);
			renderer->SetGpuBuffer(materialConstantBuffer);
			if (object->material->shader != nullptr) {
				renderer->SetShader(object->material->shader);
			} else {
				renderer->SetShader(ShaderManager::instance()->getShader("default"));
			}
		} else {
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

		m_WVPConstantBuffer->update(&m_wvp);
		renderer->SetGpuBuffer(m_WVPConstantBuffer);
		DrawObject(skyMap->mesh->models[0]);
	}
};
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
#include "Hollow/ECS/GLTFRenderable.h"
#include "GPUBufferManager.h"
#include "RenderTargetManager.h"

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
	Water* water;
	LightInfo				pointLights;
	PointLight* pointLight;
	Camera* m_Camera;
	SkyMap* skyMap;
	RenderTarget* target;
private:
	RenderApi* renderer;
private:
	WVP						m_wvp;
	WorldViewProjection		m_worldViewProjection;
	TransformBuff			transformBuff;
	LightMatrices			lightMatrices;

	GPUBuffer* m_WVPConstantBuffer;
	GPUBuffer* m_WorldViewProjectionBuffer;
	GPUBuffer* m_TransformConstantBuffer;
	GPUBuffer* materialConstantBuffer;
	GPUBuffer* lightInfoBuffer;
	GPUBuffer* boneInfo;

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
		m_WVPConstantBuffer = GPUBufferManager::instance()->create(0, sizeof(WVP));
		m_WorldViewProjectionBuffer = GPUBufferManager::instance()->create(1, sizeof(WorldViewProjection));
		m_TransformConstantBuffer = GPUBufferManager::instance()->create(2, sizeof(TransformBuff));
		materialConstantBuffer = GPUBufferManager::instance()->create(4, sizeof(MaterialData));
		lightInfoBuffer = GPUBufferManager::instance()->create(6, sizeof(LightInfo));
		boneInfo = GPUBufferManager::instance()->create(7, sizeof(Matrix4) * 100);

		target = RenderTargetManager::instance()->create(1920, 1080);

		renderer->SetViewport(0, 0, 1920, 1080);
		m_worldViewProjection.offset = 0.0f;
	}

	virtual void PreUpdate(double dt)
	{
		renderer->ClearRenderTarget(0, (float*)ClearColor);
		renderer->ClearRenderTarget(target, ClearColor);
		m_worldViewProjection.cameraPosition = m_Camera->GetPositionVec3();
	}

	virtual void Update(double dt)
	{
		//updateWVP(m_Camera);
		////DrawWater();
		//renderer->SetRenderTarget(target);
		//DrawSceneGLTF();
		//DrawSkyMap();

		updateWVP(m_Camera);
		renderer->SetRenderTarget(0);
		DrawSceneGLTF();
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
					Matrix4::Scaling(transform->scale) *
					Matrix4::Rotation(transform->rotation) *
					Matrix4::Translation(transform->position)
				);
				m_TransformConstantBuffer->update(&transformBuff);
				renderer->SetGpuBuffer(m_TransformConstantBuffer);

				if (entity.hasComponent<AnimationComponent>()) {
					AnimationComponent* animationComponent = entity.getComponent<AnimationComponent>();
					transformBuff.hasAnimation = true;
					boneInfo->update(animationComponent->boneInfo);
					renderer->SetGpuBuffer(boneInfo);
				}
				else {
					transformBuff.hasAnimation = false;
				}

				for (auto& model : renderable->mesh->models) {
					DrawObject(model);
				}
			}
		}
	}

	void DrawSceneGLTF()
	{
		for (auto& entity : EntityManager::instance()->getContainer<GameObject>()->entityList) {
			if (entity.hasComponent<GLTFRenderable>()) {
				GLTFRenderable* renderable = entity.getComponent<GLTFRenderable>();
				drawRecursive(renderable->rootNode, renderable);
			}
		}
	}

	void drawRecursive(Hollow::Node* parentNode, GLTFRenderable* renderable)
	{
		transformBuff.transform = parentNode->transformation;

		m_TransformConstantBuffer->update(&transformBuff);
		renderer->SetGpuBuffer(m_TransformConstantBuffer);

		for (auto& it : parentNode->childrens) {
			if (it->mesh != -1) {
				renderer->SetShader(ShaderManager::instance()->getShader("default"));
				renderer->SetVertexBuffer(renderable->renderables[it->mesh]->vBuffer);
				renderer->SetIndexBuffer(renderable->renderables[it->mesh]->iBuffer);
				renderer->DrawIndexed(renderable->renderables[it->mesh]->iBuffer->getSize());
			}
		}

		for (auto& it : parentNode->childrens) {
			drawRecursive(it, renderable);
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
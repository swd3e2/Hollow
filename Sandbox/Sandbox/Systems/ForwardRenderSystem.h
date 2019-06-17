#pragma once
#include "Sandbox/Components/TransformComponent.h"
#include "Hollow/ECS/System.h"
#include "Hollow/ECS/EntityManager.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Entities/Light.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Resources/Mesh/Mesh.h"
#include "Hollow/Graphics/ShaderManager.h"
#include "Sandbox/Graphics/SkyMap.h"
#include "Hollow/Input/InputManager.h"
#include "Sandbox/Components/GLTFRenderable.h"
#include "Hollow/Graphics/GPUBufferManager.h"
#include "Hollow/Graphics/RenderTargetManager.h"
#include "Hollow/Graphics/Renderer/Base/RenderApi.h"

using namespace Hollow;

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
	int pointLightsNum = 1;
};

class ForwardRenderSystem : public Hollow::System<ForwardRenderSystem>
{
public:
	LightInfo				pointLights;
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

		target = RenderTargetManager::instance()->create(1600, 900);

		renderer->SetViewport(0, 0, 1600, 900);
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

	void DrawSceneGLTF()
	{
		for (auto& entity : EntityManager::instance()->getContainer<GameObject>()->entityList) {
			if (entity.hasComponent<GLTFRenderable>() && entity.hasComponent<TransformComponent>()) {
				GLTFRenderable* renderable = entity.getComponent<GLTFRenderable>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				Draw(renderable->rootNode, renderable, transform, Matrix4::Identity());
			}
		}
	}

	void Draw(Hollow::GLTF::Node* node, GLTFRenderable* renderable, TransformComponent* transform, const Matrix4& parentTransform)
	{
		transformBuff.transform = Matrix4::Transpose(Matrix4::Scaling(transform->scale) * Matrix4::Rotation(transform->rotation) * Matrix4::Translation(transform->position)) *
			parentTransform * node->transformation;

		m_TransformConstantBuffer->update(&transformBuff);
		renderer->SetGpuBuffer(m_TransformConstantBuffer);

		if (node->mesh != -1) {
			if (renderable->renderables[node->mesh]->material->diffuseTexture != nullptr) {
				renderer->SetTexture(0, renderable->renderables[node->mesh]->material->diffuseTexture);
			}
			if (renderable->renderables[node->mesh]->material->normalTexture != nullptr) {
				renderer->SetTexture(1, renderable->renderables[node->mesh]->material->normalTexture);
			}
			if (renderable->renderables[node->mesh]->material->specularTexture != nullptr) {
				renderer->SetTexture(2, renderable->renderables[node->mesh]->material->specularTexture);
			}

			renderer->SetShader(ShaderManager::instance()->getShader("default"));
			renderer->SetVertexBuffer(renderable->renderables[node->mesh]->vBuffer);
			renderer->SetIndexBuffer(renderable->renderables[node->mesh]->iBuffer);
			renderer->DrawIndexed(renderable->renderables[node->mesh]->iBuffer->getSize());
		}

		for (auto& it : node->childrens) {
			Draw(it, renderable, transform, node->transformation);
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
			if (object->material->diffuseTexture != nullptr) {
				renderer->SetTexture(0, object->material->diffuseTexture);
			}
			if (object->material->normalTexture != nullptr) {
				renderer->SetTexture(1, object->material->normalTexture);
			}
			if (object->material->specularTexture != nullptr) {
				renderer->SetTexture(2, object->material->specularTexture);
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
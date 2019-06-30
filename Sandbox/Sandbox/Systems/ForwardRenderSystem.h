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
#include "Hollow/Graphics/GPUBufferManager.h"
#include "Hollow/Graphics/RenderTargetManager.h"
#include "Hollow/Graphics/Renderer/Base/RenderApi.h"
#include "Hollow/Common/Log.h"
#include "Sandbox/Events.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/ProjectSettings.h"
#include "Sandbox/Graphics/Shadow.h"

using namespace Hollow;

struct WVP
{
	Matrix4 WVP;
	Vector3 cameraPosition;
};

struct TransformBuff
{
	Matrix4 transform;
	Vector3 color;
	float pad;
	int selected;
	int hasAnimation;
};

struct ShadowStruct
{
	Matrix4 ShadowWVP;
};

struct LightInfo
{
	int pointLightsNum = 1;
};

class ForwardRenderSystem : public Hollow::System<ForwardRenderSystem>
{
public:
	Shadow shadow;
	LightInfo pointLights;
	Camera* m_Camera;
	SkyMap* skyMap;
	RenderTarget* main;
	RenderTarget* debug;
	int pickedID;
private:
	RenderApi* renderer;
private:
	WVP						m_wvp;
	TransformBuff			transformBuff;
	ShadowStruct shadowStruct;

	GPUBuffer* shadowConstantBuffer;
	GPUBuffer* m_WVPConstantBuffer;
	GPUBuffer* m_WorldViewProjectionBuffer;
	GPUBuffer* m_TransformConstantBuffer;
	GPUBuffer* materialConstantBuffer;
	GPUBuffer* lightInfoBuffer;
	GPUBuffer* boneInfo;

	int pointLightsNum = 0;
	int directionalLightNum = 0;
	int spotLifhtNum = 0;

	int width;
	int height;

	const UINT uavs = 0;

	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
public:
	ForwardRenderSystem(RenderApi* renderer, int width, int height) :
		renderer(renderer), width(width), height(height)
	{
		m_WVPConstantBuffer = GPUBufferManager::instance()->create(0, sizeof(WVP));
		shadowConstantBuffer = GPUBufferManager::instance()->create(1, sizeof(ShadowStruct));
		m_TransformConstantBuffer = GPUBufferManager::instance()->create(2, sizeof(TransformBuff));
		materialConstantBuffer = GPUBufferManager::instance()->create(4, sizeof(MaterialData));
		lightInfoBuffer = GPUBufferManager::instance()->create(6, sizeof(LightInfo));
		boneInfo = GPUBufferManager::instance()->create(7, sizeof(Matrix4) * 100);

		debug = RenderTargetManager::instance()->create(this->width, this->height);
		main = RenderTargetManager::instance()->create(this->width, this->height);

		shadow.renderTarget = RenderTargetManager::instance()->create(this->width, this->height);
		shadow.shadowCamera = new Camera(false);
		shadow.shadowCamera->SetProjectionValues(90, static_cast<float>(this->width) / static_cast<float>(this->height), 0.01f, 10000.0f);

		renderer->SetViewport(0, 0, this->width, this->height);
	}

	virtual void PreUpdate(double dt)
	{
		renderer->ClearRenderTarget(0, (float*)ClearColor);
		renderer->ClearRenderTarget(main, ClearColor);
		renderer->ClearRenderTarget(debug, ClearColor);
		renderer->ClearRenderTarget(shadow.renderTarget, ClearColor);
	}

	virtual void Update(double dt)
	{
		shadow.shadowCamera->Update(dt);

		if (ProjectSettings::instance()->isProjectLoaded) {
			renderer->SetRenderTarget(shadow.renderTarget);

			shadowStruct.ShadowWVP = shadow.shadowCamera->GetProjectionMatrix() * shadow.shadowCamera->GetViewMatrix();
			shadowConstantBuffer->update(&shadowStruct);
			renderer->SetGpuBuffer(shadowConstantBuffer);

			DrawDepth();

			renderer->SetRenderTarget(main);
			renderer->SetTextureDepthBuffer(3, shadow.renderTarget);
			updateWVP(this->m_Camera);

			Draw();

			DrawSkyMap();

			renderer->UnsetTexture(3);

			renderer->SetRenderTarget(0);

			/*if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_CONTROL) && InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_LEFT)) {
				Vector4 selectedColor = debug->readPixel(InputManager::mcx, InputManager::mcy);
				pickedID = selectedColor.x + selectedColor.y * 256 + selectedColor.z * 256 * 256;
				Hollow::EventSystem::instance()->addEvent(new ChangeSelectedEntity(pickedID));
			}*/
		}
	}

	virtual void PostUpdate(double dt)
	{
		renderer->Present();
	}

	void DrawDepth()
	{
		renderer->SetShader(ShaderManager::instance()->getShader("depth"));

		for (auto& entity : EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				transformBuff.transform = Matrix4::Transpose(
					Matrix4::Scaling(transform->scale)
					* Matrix4::Rotation(transform->rotation)
					* Matrix4::Translation(transform->position)
				);
				m_TransformConstantBuffer->update(&transformBuff);
				renderer->SetGpuBuffer(m_TransformConstantBuffer);

				for (auto& object : renderable->renderables) {
					renderer->SetVertexBuffer(object.vBuffer);
					renderer->SetIndexBuffer(object.iBuffer);
					renderer->DrawIndexed(object.iBuffer->getSize());
				}
			}
		}
	}

	void Draw()
	{
		renderer->SetShader(ShaderManager::instance()->getShader("default"));

		for (auto& entity : EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				transformBuff.transform = Matrix4::Transpose(
					Matrix4::Scaling(transform->scale) 
					* Matrix4::Rotation(transform->rotation) 
					* Matrix4::Translation(transform->position)
				);
				m_TransformConstantBuffer->update(&transformBuff);
				renderer->SetGpuBuffer(m_TransformConstantBuffer);

				for (auto& object : renderable->renderables) {
					Hollow::Material& material = renderable->materials[object.material];

					if (material.diffuseTexture != nullptr) {
						renderer->SetTexture(0, material.diffuseTexture);
					} else {
						renderer->UnsetTexture(0);
					}
					if (material.normalTexture != nullptr) {
						renderer->SetTexture(1, material.normalTexture);
					} else {
						renderer->UnsetTexture(1);
					}
					if (material.specularTexture != nullptr) {
						renderer->SetTexture(2, material.specularTexture);
					} else {
						renderer->UnsetTexture(2);
					}

					materialConstantBuffer->update(&material.materialData);
					renderer->SetGpuBuffer(materialConstantBuffer);

					renderer->SetVertexBuffer(object.vBuffer);
					renderer->SetIndexBuffer(object.iBuffer);
					renderer->DrawIndexed(object.iBuffer->getSize());
				}
			}
		}
	}

	void DrawSceneForPicker()
	{
		for (auto& entity : EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				transformBuff.transform = Matrix4::Transpose(Matrix4::Scaling(transform->scale) 
					* Matrix4::Rotation(transform->rotation) 
					* Matrix4::Translation(transform->position));

				m_TransformConstantBuffer->update(&transformBuff);
				renderer->SetGpuBuffer(m_TransformConstantBuffer);

				for (auto& it : renderable->renderables) {
					DrawForPicker(it);
				}
			}
		}
	}

	void DrawForPicker(RenderableObject& object)
	{
		float r = ((object.id & 0x000000FF) >> 0) / 255.0f;
		float g = ((object.id & 0x0000FF00) >> 8) / 255.0f;
		float b = ((object.id & 0x00FF0000) >> 16) / 255.0f;

		transformBuff.color = Vector3(r, g, b);
		m_TransformConstantBuffer->update(&transformBuff);
		renderer->SetGpuBuffer(m_TransformConstantBuffer);

		renderer->SetShader(ShaderManager::instance()->getShader("picker"));
		renderer->SetVertexBuffer(object.vBuffer);
		renderer->SetIndexBuffer(object.iBuffer);
		renderer->DrawIndexed(object.iBuffer->getSize());
	}

	// Update world view projection matrix
	void updateWVP(Camera* camera)
	{
		m_wvp.WVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();
		m_WVPConstantBuffer->update(&m_wvp);
		renderer->SetGpuBuffer(m_WVPConstantBuffer);
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

		renderer->SetTexture(4, skyMap->mesh->models[0]->material->diffuseTexture);
		
		renderer->SetShader(Hollow::ShaderManager::instance()->getShader("SkyMap"));
		renderer->SetVertexBuffer(skyMap->mesh->models[0]->vBuffer);
		renderer->SetIndexBuffer(skyMap->mesh->models[0]->iBuffer);
		renderer->DrawIndexed(skyMap->mesh->models[0]->iBuffer->getSize());
	}
};
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
#include "Sandbox/Entities/Terrain.h"
#include "Sandbox/Components/TerrainData.h"

using namespace Hollow;

struct WVP
{
	Matrix4 WVP;
	Vector3 cameraPosition;
};

struct PerModel
{
	Matrix4 transform;
	int selected;
	int hasAnimation;
};

struct PerMesh
{
	Vector3 color;
	float pad;
	int selected;
};

struct ShadowStruct
{
	Matrix4 ShadowWVP;
	Hollow::Vector2 texelSize;
	float bias;
};

struct LightInfo
{
	int pointLightsNum = 1;
};

class ForwardRenderSystem : public Hollow::System<ForwardRenderSystem>
{
public:
	Shadow shadow;
	Camera* m_Camera;
	SkyMap* skyMap;
	RenderTarget* main;

	RenderTarget* gBuffer;
	RenderTarget* picker;

	int pickedID;
private:
	RenderApi* renderer;
private:
	WVP						m_wvp;
	

	ShadowStruct shadowStruct;

	GPUBuffer* shadowConstantBuffer;
	GPUBuffer* m_WVPConstantBuffer;
	GPUBuffer* m_WorldViewProjectionBuffer;
	GPUBuffer* materialConstantBuffer;
	GPUBuffer* lightInfoBuffer;
	GPUBuffer* boneInfo;

	GPUBuffer* perModel;
	GPUBuffer* perMesh;
	PerModel perModelData;
	PerMesh perMeshData;

	int pointLightsNum = 0;
	int directionalLightNum = 0;
	int spotLifhtNum = 0;

	int width;
	int height;

	const UINT uavs = 0;

	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	Hollow::VertexBuffer* quadVB;
	Hollow::IndexBuffer* quadIB;
public:
	ForwardRenderSystem(RenderApi* renderer, int width, int height) :
		renderer(renderer), width(width), height(height)
	{
		// Gpu buffers
		{
			m_WVPConstantBuffer		= GPUBufferManager::instance()->create(0, sizeof(WVP));
			shadowConstantBuffer	= GPUBufferManager::instance()->create(1, sizeof(ShadowStruct));

			perModel				= GPUBufferManager::instance()->create(2, sizeof(PerModel));
			perMesh					= GPUBufferManager::instance()->create(3, sizeof(PerMesh));

			materialConstantBuffer	= GPUBufferManager::instance()->create(4, sizeof(MaterialData));
			lightInfoBuffer			= GPUBufferManager::instance()->create(6, sizeof(LightInfo));
			boneInfo				= GPUBufferManager::instance()->create(7, sizeof(Matrix4) * 100);
		}
		
		// Render targets
		Hollow::RENDER_TARGET_DESC desc;
		desc.count = 3;
		desc.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R32G32B32A32;
		gBuffer = RenderTargetManager::instance()->create(this->width, this->height, desc);

		Hollow::RENDER_TARGET_DESC desc2;
		desc2.count = 1;
		desc2.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8;
		main = RenderTargetManager::instance()->create(this->width, this->height, desc2);

		shadow.renderTarget = RenderTargetManager::instance()->create(this->width, this->height, desc2);
		shadow.shadowCamera = new Camera(false);
		shadow.shadowCamera->SetOrthoValues(-1000, 1000, -1000, 1000, -1000, 2000);
		shadow.texelSize = Hollow::Vector2(1.0f / this->width, 1.0f / this->height);

		picker = RenderTargetManager::instance()->create(this->width, this->height, desc2);

		std::vector<Hollow::Vertex> vertices;
		vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f));
		vertices.push_back(Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f));
		quadVB = Hollow::HardwareBufferManager::instance()->createVertexBuffer(vertices.data(), 4);

		unsigned int indices[] = {
			0, 1, 2,
			2, 1, 3
		};
		quadIB = Hollow::HardwareBufferManager::instance()->createIndexBuffer(indices, 6);

		renderer->SetViewport(0, 0, this->width, this->height);
		renderer->SetDepthTestFunction(DEPTH_TEST_FUNCTION::LESS);
	}

	virtual void PreUpdate(double dt)
	{
		renderer->ClearRenderTarget(0, (float*)ClearColor);
		renderer->ClearRenderTarget(main, ClearColor);
		renderer->ClearRenderTarget(gBuffer, ClearColor);
		renderer->ClearRenderTarget(shadow.renderTarget, ClearColor);
	}

	virtual void Update(double dt)
	{
		shadow.shadowCamera->Update(dt);

		if (ProjectSettings::instance()->isProjectLoaded) {
			updateWVP(this->m_Camera);
			//// picker pass
			//{
			//	renderer->SetRenderTarget(picker);
			//	renderer->SetShader(ShaderManager::instance()->getShader("picker"));
			//	
			//	for (auto& entity : EntityManager::instance()->container<GameObject>()) {
			//		if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
			//			RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
			//			TransformComponent* transform = entity.getComponent<TransformComponent>();

			//			perModelData.transform = Matrix4::Transpose(
			//				Matrix4::Scaling(transform->scale)
			//				* Matrix4::Rotation(transform->rotation)
			//				* Matrix4::Translation(transform->position)
			//			);
			//			perModel->update(&perModelData);
			//			renderer->SetGpuBuffer(perModel);

			//			for (auto& object : renderable->renderables) {

			//				float r = ((object.id & 0x000000FF) >> 0) / 255.0f;
			//				float g = ((object.id & 0x0000FF00) >> 8) / 255.0f;
			//				float b = ((object.id & 0x00FF0000) >> 16) / 255.0f;

			//				perMeshData.color = Vector3(r, g, b);
			//				perMesh->update(&perMeshData);
			//				renderer->SetGpuBuffer(perMesh);

			//				renderer->SetVertexBuffer(object.vBuffer);
			//				renderer->SetIndexBuffer(object.iBuffer);
			//				renderer->DrawIndexed(object.iBuffer->getSize());
			//			}
			//		}
			//	}
			//}
			// GBuffer pass
			{
				renderer->SetRenderTarget(gBuffer);
				renderer->SetShader(ShaderManager::instance()->getShader("gbuffer"));
				renderer->SetCullMode(Hollow::CULL_MODE::CULL_BACK);

				for (auto& entity : EntityManager::instance()->container<GameObject>()) {
					if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
						RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
						TransformComponent* transform = entity.getComponent<TransformComponent>();

						perModelData.transform = Matrix4::Transpose(
							Matrix4::Scaling(transform->scale)
							* Matrix4::Rotation(transform->rotation)
							* Matrix4::Translation(transform->position)
						);
						perModel->update(&perModelData);
						renderer->SetGpuBuffer(perModel);

						for (auto& object : renderable->renderables) {
							Hollow::Material& material = renderable->materials[object.material];
							if (material.diffuseTexture != nullptr) {
								renderer->SetTexture(0, material.diffuseTexture);
							} else {
								renderer->UnsetTexture(0);
							}
							renderer->SetVertexBuffer(object.vBuffer);
							renderer->SetIndexBuffer(object.iBuffer);
							renderer->DrawIndexed(object.iBuffer->getSize());
						}
					}
				}
				for (auto& entity : EntityManager::instance()->container<Terrain>()) {
					if (entity.hasComponent<TerrainData>()) {
						TerrainData* data = entity.getComponent<TerrainData>();

						perModelData.transform = Matrix4::Identity();
						perModel->update(&perModelData);
						renderer->SetGpuBuffer(perModel);

						renderer->SetVertexBuffer(data->vBuffer);
						renderer->SetIndexBuffer(data->iBuffer);
						renderer->DrawIndexed(data->iBuffer->getSize());
					}
				}
			}
			// Shadow pass
			{
				shadowStruct.ShadowWVP = shadow.shadowCamera->GetProjectionMatrix() * shadow.shadowCamera->GetViewMatrix();
				shadowStruct.texelSize = shadow.texelSize;
				shadowStruct.bias = shadow.bias;
				shadowConstantBuffer->update(&shadowStruct);
				renderer->SetGpuBuffer(shadowConstantBuffer);

				renderer->SetRenderTarget(shadow.renderTarget);
				renderer->SetShader(ShaderManager::instance()->getShader("depth"));

				for (auto& entity : EntityManager::instance()->container<GameObject>()) {
					if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
						RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
						TransformComponent* transform = entity.getComponent<TransformComponent>();

						perModelData.transform = Matrix4::Transpose(
							Matrix4::Scaling(transform->scale)
							* Matrix4::Rotation(transform->rotation)
							* Matrix4::Translation(transform->position)
						);
						perModel->update(&perModelData);
						renderer->SetGpuBuffer(perModel);

						for (auto& object : renderable->renderables) {
							Hollow::Material& material = renderable->materials[object.material];
							renderer->SetVertexBuffer(object.vBuffer);
							renderer->SetIndexBuffer(object.iBuffer);
							renderer->DrawIndexed(object.iBuffer->getSize());
						}
					}
				}
			}
			// Light pass
			{
				renderer->SetViewport(0, 0, this->width, this->height);
				renderer->SetCullMode(Hollow::CULL_MODE::CULL_BACK);
				renderer->SetRenderTarget(0);
				renderer->SetTextureColorBuffer(0, gBuffer, 0);
				renderer->SetTextureColorBuffer(1, gBuffer, 1);
				renderer->SetTextureColorBuffer(2, gBuffer, 2);
				renderer->SetTextureDepthBuffer(3, shadow.renderTarget);
				renderer->SetTextureDepthBuffer(5, gBuffer);

				renderer->SetShader(ShaderManager::instance()->getShader("light"));
				renderer->SetVertexBuffer(quadVB);
				renderer->SetIndexBuffer(quadIB);
				renderer->DrawIndexed(6);

				renderer->SetDepthTestFunction(DEPTH_TEST_FUNCTION::GREATER);
				renderer->SetCullMode(Hollow::CULL_MODE::CULL_FRONT);
				DrawSkyMap();
				renderer->SetDepthTestFunction(DEPTH_TEST_FUNCTION::LESS);

				renderer->UnsetTexture(0);
				renderer->UnsetTexture(1);
				renderer->UnsetTexture(2);
				renderer->UnsetTexture(3);
				renderer->UnsetTexture(5);
			}

			if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_CONTROL) && InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_LEFT)) {
				Vector4 selectedColor = picker->readPixel(InputManager::mcx, InputManager::mcy);
				pickedID = selectedColor.x + selectedColor.y * 256 + selectedColor.z * 256 * 256;
				Hollow::EventSystem::instance()->addEvent(new ChangeSelectedEntity(pickedID));
			}
		}
	}

	virtual void PostUpdate(double dt)
	{
		renderer->Present();
	}

	void DrawSceneForPicker()
	{
		for (auto& entity : EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				perModelData.transform = Matrix4::Transpose(Matrix4::Scaling(transform->scale)
					* Matrix4::Rotation(transform->rotation) 
					* Matrix4::Translation(transform->position));

				perModel->update(&perModelData);
				renderer->SetGpuBuffer(perModel);

				for (auto& it : renderable->renderables) {
					DrawForPicker(it);
				}
			}
		}
	}

	void DrawForPicker(RenderableObject& object)
	{
		perModel->update(&perModelData);
		renderer->SetGpuBuffer(perModel);

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
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
#include "Hollow/Graphics/Base/RenderApi.h"
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

	PipelineState* gBufferPipeline; 
	PipelineState* depthPipeline;
	PipelineState* lightPipeline;
	PipelineState* skyMapPipeline;

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
		// Shaders
		{
			Hollow::INPUT_LAYOUT_DESC layoutDesc = {
				{ Hollow::INPUT_DATA_TYPE::Float3, "POSITION" }, // pos
				{ Hollow::INPUT_DATA_TYPE::Float2, "TEXCOORD" }, // texcoord
				{ Hollow::INPUT_DATA_TYPE::Float3, "NORMAL" }, // normal
				{ Hollow::INPUT_DATA_TYPE::Float3, "TANGENT" }, // tangent
				{ Hollow::INPUT_DATA_TYPE::Float3, "BITANGENT" }, // bitangent 
			};

			Hollow::InputLayout* layout = Hollow::InputLayout::create(layoutDesc);

			renderer->SetLayout(layout);

			if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
				{
					Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/gbuffer.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/gbuffer.hlsl"), "main" });

					gBufferPipeline = Hollow::PipelineState::create(pipelineDesc);
				}
				{
					Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/depth.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/depth.hlsl"), "main" });

					depthPipeline = Hollow::PipelineState::create(pipelineDesc);
				}
				{
					Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/light.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/light.hlsl"), "main" });

					lightPipeline = Hollow::PipelineState::create(pipelineDesc);
				}
				{
					Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/SkyMap.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/SkyMap.hlsl"), "main" });

					skyMapPipeline = Hollow::PipelineState::create(pipelineDesc);
				}
			} else {
				{
					Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/gbuffer.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/gbuffer.glsl"), "main" });

					gBufferPipeline = Hollow::PipelineState::create(pipelineDesc);
				}
				{
					Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/depth.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/depth.glsl"), "main" });

					depthPipeline = Hollow::PipelineState::create(pipelineDesc);
				}
				{
					Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/light.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/light.glsl"), "main" });

					lightPipeline = Hollow::PipelineState::create(pipelineDesc);
				}
				{
					Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/SkyMap.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/SkyMap.glsl"), "main" });

					skyMapPipeline = Hollow::PipelineState::create(pipelineDesc);
				}
			}
			
		}
		// Render targets
		Hollow::RENDER_TARGET_DESC desc;
		desc.count = 3;
		desc.width = this->width;
		desc.height = this->height;
		desc.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R32G32B32A32;
		gBuffer = RenderTargetManager::instance()->create(desc);

		Hollow::RENDER_TARGET_DESC desc2;
		desc2.count = 1;
		desc2.width = this->width;
		desc2.height = this->height;
		desc2.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8;
		main = RenderTarget::create(desc2);

		shadow.renderTarget = RenderTargetManager::instance()->create(desc2);
		shadow.shadowCamera = new Camera(false);
		shadow.shadowCamera->SetOrthoValues(-1000, 1000, -1000, 1000, -1000, 2000);
		shadow.texelSize = Hollow::Vector2(1.0f / this->width, 1.0f / this->height);

		picker = RenderTarget::create(desc2);

		std::vector<Hollow::Vertex> vertices;			
		vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1.0f, 0.0f));				
		vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f));				
		vertices.push_back(Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f));				
		vertices.push_back(Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f));

		quadVB = Hollow::VertexBuffer::create({ vertices.data(), vertices.size(), sizeof(Hollow::Vertex) });

		unsigned int indices[] = {
			0, 1, 2,
			2, 1, 3
		};

		quadIB = Hollow::IndexBuffer::create({ indices, 6, sizeof(Hollow::Vertex) });

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
				//renderer->SetShader(ShaderManager::instance()->getShader("gbuffer"));
				renderer->SetPipelineState(gBufferPipeline);
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
							renderer->DrawIndexed(object.iBuffer->mHardwareBuffer->getSize());
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
						renderer->DrawIndexed(data->iBuffer->mHardwareBuffer->getSize());
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
				renderer->SetPipelineState(depthPipeline);

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
							renderer->DrawIndexed(object.iBuffer->mHardwareBuffer->getSize());
						}
					}
				}
			}
			// Light pass
			{
				renderer->SetViewport(0, 0, this->width, this->height);
				renderer->SetCullMode(Hollow::CULL_MODE::CULL_NONE);
				renderer->SetRenderTarget(0);
				renderer->SetTextureColorBuffer(0, gBuffer, 0);
				renderer->SetTextureColorBuffer(1, gBuffer, 1);
				renderer->SetTextureColorBuffer(2, gBuffer, 2);
				renderer->SetTextureDepthBuffer(3, shadow.renderTarget);
				renderer->SetTextureDepthBuffer(5, gBuffer);

				renderer->SetPipelineState(lightPipeline);

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

		//renderer->SetShader(ShaderManager::instance()->getShader("picker"));
		renderer->SetVertexBuffer(object.vBuffer);
		renderer->SetIndexBuffer(object.iBuffer);
		renderer->DrawIndexed(object.iBuffer->mHardwareBuffer->getSize());
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
		
		renderer->SetPipelineState(skyMapPipeline);
		
		renderer->SetVertexBuffer(skyMap->mesh->models[0]->vBuffer);
		renderer->SetIndexBuffer(skyMap->mesh->models[0]->iBuffer);
		renderer->DrawIndexed(skyMap->mesh->models[0]->iBuffer->mHardwareBuffer->getSize());
	}
};
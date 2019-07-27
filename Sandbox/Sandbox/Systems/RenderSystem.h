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

class RenderSystem : public Hollow::System<RenderSystem>
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
	
	Hollow::InputLayout* layout;

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
	PipelineState* defaultPipeline;
	PipelineState* terrainPipeline;

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
	RenderSystem(RenderApi* renderer, int width, int height) :
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
				{ Hollow::INPUT_DATA_TYPE::Int4, "BONE" },
				{ Hollow::INPUT_DATA_TYPE::Float4, "WEIGHT" }
			};
			
			layout = Hollow::InputLayout::create(layoutDesc);

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
				{
					Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/terrain.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/terrain.hlsl"), "main" });

					terrainPipeline = Hollow::PipelineState::create(pipelineDesc);
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
		desc2.width = 4096;
		desc2.height = 4096;
		desc2.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8;
		main = RenderTarget::create(desc2);

		shadow.renderTarget = RenderTargetManager::instance()->create(desc2);
		shadow.shadowCamera = new Camera(false);
		shadow.shadowCamera->setOrthoValues(-1000, 1000, -1000, 1000, -1000, 2000);
		shadow.texelSize = Hollow::Vector2(1.0f / this->width, 1.0f / this->height);

		std::vector<Hollow::Vertex> vertices;			
		vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1.0f, 0.0f));				
		vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f));				
		vertices.push_back(Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f));				
		vertices.push_back(Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f));

		quadVB = Hollow::VertexBuffer::create({ vertices.data(), vertices.size(), sizeof(Vertex) });

		unsigned int indices[] = {
			0, 1, 2,
			2, 1, 3
		};

		quadIB = Hollow::IndexBuffer::create({ indices, 6, Hollow::INDEX_FORMAT::UINT });

		renderer->setViewport(0, 0, this->width, this->height);
		renderer->setDepthTestFunction(DEPTH_TEST_FUNCTION::LESS);
	}

	virtual void PreUpdate(double dt)
	{
		renderer->clearRenderTarget(0, (float*)ClearColor);
		renderer->clearRenderTarget(main, ClearColor);
		renderer->clearRenderTarget(gBuffer, ClearColor);
		renderer->clearRenderTarget(shadow.renderTarget, ClearColor);
	}

	virtual void Update(double dt)
	{
		renderer->setInputLayout(layout);
		shadow.shadowCamera->update(dt);

		if (ProjectSettings::instance()->isProjectLoaded) {
			updateWVP(this->m_Camera);

			// GBuffer pass
			{
				renderer->setRenderTarget(gBuffer);
				renderer->setPipelineState(gBufferPipeline);
				renderer->setCullMode(Hollow::CULL_MODE::CULL_BACK);

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
						renderer->setGpuBuffer(perModel);

						for (auto& object : renderable->renderables) {
							Hollow::Material& material = renderable->materials[object.material];
							if (material.diffuseTexture != nullptr) {
								renderer->setTexture(0, material.diffuseTexture);
							} else {
								renderer->unsetTexture(0);
							}
							renderer->setVertexBuffer(object.vBuffer);
							renderer->setIndexBuffer(object.iBuffer);
							renderer->drawIndexed(object.iBuffer->mHardwareBuffer->getSize());
						}
					}
				}
				for (auto& entity : EntityManager::instance()->container<Terrain>()) {
					if (entity.hasComponent<TransformComponent>() && entity.hasComponent<TerrainData>()) {
						TerrainData* data = entity.getComponent<TerrainData>();
						TransformComponent* transform = entity.getComponent<TransformComponent>();

						if (data->vBuffer != nullptr) {
							perModelData.transform = Matrix4::Transpose(
								Matrix4::Scaling(transform->scale)
								* Matrix4::Rotation(transform->rotation)
								* Matrix4::Translation(transform->position)
							);

							perModel->update(&perModelData);
							renderer->setGpuBuffer(perModel);
							renderer->setTexture(0, data->tex);
							renderer->setVertexBuffer(data->vBuffer);
							renderer->draw(data->vBuffer->mHardwareBuffer->getSize());
						}
					}
				}
			}
			// Shadow pass
			{
				renderer->setViewport(0, 0, 4096, 4096);

				shadowStruct.ShadowWVP = shadow.shadowCamera->getProjectionMatrix() * shadow.shadowCamera->getViewMatrix();
				shadowStruct.texelSize = shadow.texelSize;
				shadowStruct.bias = shadow.bias;
				shadowConstantBuffer->update(&shadowStruct);
				renderer->setGpuBuffer(shadowConstantBuffer);

				renderer->setRenderTarget(shadow.renderTarget);
				renderer->setPipelineState(depthPipeline);

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
						renderer->setGpuBuffer(perModel);

						for (auto& object : renderable->renderables) {
							Hollow::Material& material = renderable->materials[object.material];
							renderer->setVertexBuffer(object.vBuffer);
							renderer->setIndexBuffer(object.iBuffer);
							renderer->drawIndexed(object.iBuffer->mHardwareBuffer->getSize());
						}
					}
				}
				for (auto& entity : EntityManager::instance()->container<Terrain>()) {
					if (entity.hasComponent<TransformComponent>() && entity.hasComponent<TerrainData>()) {
						TerrainData* data = entity.getComponent<TerrainData>();
						TransformComponent* transform = entity.getComponent<TransformComponent>();

						if (data->vBuffer != nullptr) {
							perModelData.transform = Matrix4::Transpose(
								Matrix4::Scaling(transform->scale)
								* Matrix4::Rotation(transform->rotation)
								* Matrix4::Translation(transform->position)
							);

							perModel->update(&perModelData);
							renderer->setGpuBuffer(perModel);
							renderer->setTexture(0, data->tex);
							renderer->setVertexBuffer(data->vBuffer);
							renderer->draw(data->vBuffer->mHardwareBuffer->getSize());
						}
					}
				}
			}
			// Light pass
			{
				renderer->setViewport(0, 0, this->width, this->height);
				renderer->setCullMode(Hollow::CULL_MODE::CULL_BACK);
				renderer->setRenderTarget(0);
				renderer->setTextureColorBuffer(0, gBuffer, 0);
				renderer->setTextureColorBuffer(1, gBuffer, 1);
				renderer->setTextureColorBuffer(2, gBuffer, 2);
				renderer->setTextureDepthBuffer(3, shadow.renderTarget);
				renderer->setTextureDepthBuffer(5, gBuffer);

				renderer->setPipelineState(lightPipeline);

				renderer->setVertexBuffer(quadVB);
				renderer->setIndexBuffer(quadIB);
				renderer->drawIndexed(6);

				renderer->setDepthTestFunction(DEPTH_TEST_FUNCTION::GREATER);
				renderer->setCullMode(Hollow::CULL_MODE::CULL_FRONT);
				DrawSkyMap();
				renderer->setDepthTestFunction(DEPTH_TEST_FUNCTION::LESS);

				renderer->unsetTexture(0);
				renderer->unsetTexture(1);
				renderer->unsetTexture(2);
				renderer->unsetTexture(3);
				renderer->unsetTexture(5);
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
		renderer->present();
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
				renderer->setGpuBuffer(perModel);

				for (auto& it : renderable->renderables) {
					DrawForPicker(it);
				}
			}
		}
	}

	void DrawForPicker(RenderableObject& object)
	{
		perModel->update(&perModelData);
		renderer->setGpuBuffer(perModel);

		//renderer->SetShader(ShaderManager::instance()->getShader("picker"));
		renderer->setVertexBuffer(object.vBuffer);
		renderer->setIndexBuffer(object.iBuffer);
		renderer->drawIndexed(object.iBuffer->mHardwareBuffer->getSize());
	}

	// Update world view projection matrix
	void updateWVP(Camera* camera)
	{
		m_wvp.WVP = camera->getProjectionMatrix() * camera->getViewMatrix();
		m_WVPConstantBuffer->update(&m_wvp);
		renderer->setGpuBuffer(m_WVPConstantBuffer);
	}

	void DrawSkyMap()
	{
		Matrix4 viewMatrx = m_Camera->getViewMatrix();
		viewMatrx.md[0][3] = 0.0f;
		viewMatrx.md[1][3] = 0.0f;
		viewMatrx.md[2][3] = 0.0f;
		m_wvp.WVP = m_Camera->getProjectionMatrix() * viewMatrx;

		m_WVPConstantBuffer->update(&m_wvp);
		renderer->setGpuBuffer(m_WVPConstantBuffer);

		renderer->setTexture(4, skyMap->mesh->models[0]->material->diffuseTexture);
		
		renderer->setPipelineState(skyMapPipeline);
		
		renderer->setVertexBuffer(skyMap->mesh->models[0]->vBuffer);
		renderer->setIndexBuffer(skyMap->mesh->models[0]->iBuffer);
		renderer->drawIndexed(skyMap->mesh->models[0]->iBuffer->mHardwareBuffer->getSize());
	}
};
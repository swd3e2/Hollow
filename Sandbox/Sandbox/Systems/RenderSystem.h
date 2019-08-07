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
#include "Sandbox/Components/AmbientLightComponent.h"
#include "Sandbox/Components/DiffuseLightComponent.h"
#include "Sandbox/Components/SpotLightComponent.h"
#include "Sandbox/Components/PointLightComponent.h"

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
	AmbientLightData ambient[100];
	DiffuseLightData diffuse[100];
	SpotLightData spot[100];
	PointLightData point[100];
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
	int culled = 0;
private:
	RenderApi* renderer;
private:
	WVP						m_wvp;
	
	Hollow::InputLayout* defaultLayout;
	Hollow::InputLayout* terrainLayout;

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

	Hollow::Vector4 AABBplane[6];
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
			lightInfoBuffer			= GPUBufferManager::instance()->create(5, sizeof(LightInfo));
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
				{ Hollow::INPUT_DATA_TYPE::Int4,   "BONE" },
				{ Hollow::INPUT_DATA_TYPE::Float4, "WEIGHT" }
			};
			
			defaultLayout = Hollow::InputLayout::create(layoutDesc);

			Hollow::INPUT_LAYOUT_DESC terrainLayoutDesc = {
				{ Hollow::INPUT_DATA_TYPE::Float3, "POSITION" }, // pos
				{ Hollow::INPUT_DATA_TYPE::Float2, "TEXCOORD" }, // texcoord
				{ Hollow::INPUT_DATA_TYPE::Float3, "NORMAL" }, // normal
				{ Hollow::INPUT_DATA_TYPE::Float3, "COLOR" }, // color
			};

			terrainLayout = Hollow::InputLayout::create(terrainLayoutDesc);

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
				{
					Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/terrain.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/terrain.glsl"), "main" });

					terrainPipeline = Hollow::PipelineState::create(pipelineDesc);
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
		shadow.texelSize = Hollow::Vector2(1.0f / desc2.width, 1.0f / desc2.height);
		shadow.bias = 0.002f;

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
		culled = 0;
		renderer->setInputLayout(defaultLayout);
		shadow.shadowCamera->update(dt);
		calculateAABBPlane();

		if (ProjectSettings::instance()->isProjectLoaded) {
			updateWVP(this->m_Camera);

			// GBuffer pass
			{
				renderer->setRenderTarget(gBuffer);
				renderer->setPipelineState(gBufferPipeline);
				renderer->setCullMode(Hollow::CULL_MODE::CULL_NONE);

				for (auto& entity : EntityManager::instance()->container<GameObject>()) {
					if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
						RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
						TransformComponent* transform = entity.getComponent<TransformComponent>();

						Hollow::Matrix4 trs = Matrix4::Scaling(transform->scale)
							* Matrix4::Rotation(transform->rotation)
							* Matrix4::Translation(transform->position);

						Hollow::Vector3 A = trs * renderable->A;
						Hollow::Vector3 B = trs * renderable->B;

						if (!cull(transform->position, A, B)) {
							perModelData.transform = Matrix4::Transpose(trs);
							perModel->update(&perModelData);
							renderer->setGpuBuffer(perModel);

							for (auto& object : renderable->renderables) {
								Hollow::Material& material = renderable->materials[object.material];
								if (material.diffuseTexture != nullptr) {
									renderer->setTexture(0, material.diffuseTexture);
								} else {
									renderer->unsetTexture(0);
								}
								materialConstantBuffer->update(&renderable->materials[object.material].materialData);
								renderer->setGpuBuffer(materialConstantBuffer);

								renderer->setVertexBuffer(object.vBuffer);
								renderer->setIndexBuffer(object.iBuffer);
								renderer->drawIndexed(object.iBuffer->mHardwareBuffer->getSize());
							}
						} else {
							culled++;
						}
					}
				}
				renderer->setInputLayout(terrainLayout);
				renderer->setPipelineState(terrainPipeline);
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
				renderer->setInputLayout(defaultLayout);
			}
			// Shadow pass
			{
				renderer->setViewport(0, 0, 4096, 4096);
				renderer->setCullMode(Hollow::CULL_MODE::CULL_FRONT);

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

			/*if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_CONTROL) && InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_LEFT)) {
				Vector4 selectedColor = picker->readPixel(InputManager::mcx, InputManager::mcy);
				pickedID = selectedColor.x + selectedColor.y * 256 + selectedColor.z * 256 * 256;
				Hollow::EventSystem::instance()->addEvent(new ChangeSelectedEntity(pickedID));
			}*/
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

	void calculateAABBPlane()
	{
		Hollow::Matrix4 projection = Hollow::Matrix4::Transpose(m_Camera->getProjectionMatrix() * m_Camera->getViewMatrix());

		// Left Frustum Plane
		// Add first column of the matrix to the fourth column
		AABBplane[0].x = projection.md[0][3] + projection.md[0][0];
		AABBplane[0].y = projection.md[1][3] + projection.md[1][0];
		AABBplane[0].z = projection.md[2][3] + projection.md[2][0];
		AABBplane[0].w = projection.md[3][3] + projection.md[3][0];

		// Right Frustum Plane
		// Subtract first column of matrix from the fourth column
		AABBplane[1].x = projection.md[0][3] - projection.md[0][0];
		AABBplane[1].y = projection.md[1][3] - projection.md[1][0];
		AABBplane[1].z = projection.md[2][3] - projection.md[2][0];
		AABBplane[1].w = projection.md[3][3] - projection.md[3][0];

		// Top Frustum Plane
		// Subtract second column of matrix from the fourth column
		AABBplane[2].x = projection.md[0][3] - projection.md[0][1];
		AABBplane[2].y = projection.md[1][3] - projection.md[1][1];
		AABBplane[2].z = projection.md[2][3] - projection.md[2][1];
		AABBplane[2].w = projection.md[3][3] - projection.md[3][1];

		// Bottom Frustum Plane
		// Add second column of the matrix to the fourth column
		AABBplane[3].x = projection.md[0][3] + projection.md[0][1];
		AABBplane[3].y = projection.md[1][3] + projection.md[1][1];
		AABBplane[3].z = projection.md[2][3] + projection.md[2][1];
		AABBplane[3].w = projection.md[3][3] + projection.md[3][1];

		// Near Frustum Plane
		// We could add the third column to the fourth column to get the near plane,
		// but we don't have to do this because the third column IS the near plane
		AABBplane[4].x = projection.md[0][2];
		AABBplane[4].y = projection.md[1][2];
		AABBplane[4].z = projection.md[2][2];
		AABBplane[4].w = projection.md[3][2];

		// Far Frustum Plane
		// Subtract third column of matrix from the fourth column
		AABBplane[5].x = projection.md[0][3] - projection.md[0][2];
		AABBplane[5].y = projection.md[1][3] - projection.md[1][2];
		AABBplane[5].z = projection.md[2][3] - projection.md[2][2];
		AABBplane[5].w = projection.md[3][3] - projection.md[3][2];

		for (int i = 0; i < 6; i++) {
			float length = sqrt((AABBplane[i].x * AABBplane[i].x) + (AABBplane[i].y * AABBplane[i].y) + (AABBplane[i].z * AABBplane[i].z));
			AABBplane[i].x /= length;
			AABBplane[i].y /= length;
			AABBplane[i].z /= length;
			AABBplane[i].w /= length;
		}
	}

	bool cull(const Hollow::Vector3& position, const Hollow::Vector3& A, const Hollow::Vector3& B)
	{
		// Loop through each frustum plane
		for (int i = 0; i < 6; ++i) {

			if (Hollow::Vector3::dot(Hollow::Vector3(AABBplane[i].x, AABBplane[i].y, AABBplane[i].z), position + ((B - A))) + AABBplane[i].w < 0.0f) {
				return true;
			}
		}
		return false;
	}
};
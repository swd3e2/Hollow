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
#include "Hollow/Graphics/RenderApi.h"
#include "Hollow/Common/Log.h"
#include "Sandbox/Events.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/ProjectSettings.h"
#include "Sandbox/Graphics/Shadow.h"
#include "Sandbox/Entities/Terrain.h"
#include "Sandbox/Components/TerrainData.h"
#include "Sandbox/Entities/Light.h"
#include <Hollow/Test.h>
#include <Hollow/Common/Timer.h>
#include "Sandbox/Components/LightComponent.h"
#include "Sandbox/Components/AnimationComponent.h"
#include "Sandbox/Profiler.h"
#include "Sandbox/ShaderManager.h"
#include "Sandbox/Systems/PhysicsSystem.h"

struct WVP
{
	Hollow::Matrix4 WVP;
	Hollow::Vector3 cameraPosition;
	Hollow::Vector3 cameraViewDirection;
};

struct PerModel
{
	Hollow::Matrix4 transform;
	Hollow::Vector3 color;
	int selected;
	int hasAnimation;
};

struct PerMesh
{
	Hollow::Vector3 color;
	int selected;
};

struct ShadowStruct
{
	Hollow::Matrix4 ShadowWVP;
	Hollow::Vector2 texelSize;
	float bias;
};

struct LightInfo
{
	LightData lightData[100];
	int numLights;
};

class RenderSystem : public Hollow::System<RenderSystem>
{
public:
	Shadow shadow;
	Hollow::Camera* m_Camera;
	SkyMap* skyMap;
	Hollow::s_ptr<Hollow::RenderTarget> main;
	Hollow::s_ptr<Hollow::RenderTarget> gBuffer;
	Hollow::s_ptr<Hollow::RenderTarget> picker;

	int pickedID;
	int culled = 0;
	double gbufferTime;
	double shadowTime;
	double pickerTime;
	double lightTime;
	Hollow::s_ptr<Hollow::VertexBuffer> lineVB;
private:
	Hollow::RenderApi* renderer;
private:
	Hollow::Timer timer;
	WVP m_wvp;

	Hollow::s_ptr<Hollow::InputLayout> defaultLayout;
	Hollow::s_ptr<Hollow::InputLayout> terrainLayout;

	ShadowStruct shadowStruct;

	Hollow::s_ptr<Hollow::GPUBuffer> shadowConstantBuffer;
	Hollow::s_ptr<Hollow::GPUBuffer> m_WVPConstantBuffer;
	Hollow::s_ptr<Hollow::GPUBuffer> m_WorldViewProjectionBuffer;
	Hollow::s_ptr<Hollow::GPUBuffer> materialConstantBuffer;
	Hollow::s_ptr<Hollow::GPUBuffer> lightInfoBuffer;
	Hollow::s_ptr<Hollow::GPUBuffer> boneInfo;

	Hollow::s_ptr<Hollow::GPUBuffer> perModel;
	Hollow::s_ptr<Hollow::GPUBuffer> perMesh;
	PerModel perModelData;
	PerMesh perMeshData;

	Hollow::s_ptr<Hollow::ShaderPipeline> gBufferPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> depthPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> lightPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> skyMapPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> defaultPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> terrainPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> pickerPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> flatColor;

	int pointLightsNum = 0;
	int directionalLightNum = 0;
	int spotLifhtNum = 0;

	int width;
	int height;

	const UINT uavs = 0;

	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	Hollow::s_ptr<Hollow::VertexBuffer>		quadVB;
	Hollow::s_ptr<Hollow::IndexBuffer>		quadIB;


	Hollow::Vector4 AABBplane[6];
	Hollow::s_ptr<Hollow::VertexBuffer> lightVertexBuffer;
	Hollow::s_ptr<Hollow::IndexBuffer> lightIndexBuffer;
	Hollow::Material lightMaterial;
	LightInfo lightInfo;
	Hollow::Matrix4 viewProjection;

	Hollow::s_ptr<Hollow::DepthStencil> less;
	Hollow::s_ptr<Hollow::DepthStencil> greater;

	Hollow::s_ptr<Hollow::RasterizerState> cullBack;
	Hollow::s_ptr<Hollow::RasterizerState> cullFront;
	Hollow::s_ptr<Hollow::RasterizerState> cullBackWF;
	Hollow::s_ptr<Hollow::RasterizerState> cullNone;

	Hollow::s_ptr<Hollow::SamplerState> sampler;
	Hollow::s_ptr<Hollow::SamplerState> renderTargetSampler;
	Hollow::Material* defaultMaterial;

	ShaderManager shaderManager;
public:
	RenderSystem(Hollow::RenderApi* renderer, int width, int height) :
		renderer(renderer), width(width), height(height)
	{
		// Gpu buffers
		{
			m_WVPConstantBuffer			= Hollow::GPUBuffer::create(0, sizeof(WVP));
			shadowConstantBuffer		= Hollow::GPUBuffer::create(1, sizeof(ShadowStruct));
			perModel					= Hollow::GPUBuffer::create(2, sizeof(PerModel));
			perMesh						= Hollow::GPUBuffer::create(3, sizeof(PerMesh));
			materialConstantBuffer		= Hollow::GPUBuffer::create(4, sizeof(Hollow::MaterialData));
			lightInfoBuffer				= Hollow::GPUBuffer::create(5, sizeof(LightInfo));
			boneInfo					= Hollow::GPUBuffer::create(6, sizeof(Hollow::Matrix4) * 200);
		}
		// Shaders
		{
			Hollow::INPUT_LAYOUT_DESC layoutDesc = {
				{ Hollow::INPUT_DATA_TYPE::Float3, "POSITION" },
				{ Hollow::INPUT_DATA_TYPE::Float2, "TEXCOORD" },
				{ Hollow::INPUT_DATA_TYPE::Float3, "NORMAL" },
				{ Hollow::INPUT_DATA_TYPE::Float3, "TANGENT" },
				{ Hollow::INPUT_DATA_TYPE::Float3, "BITANGENT" }, 
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

			std::string baseShaderPath;
			std::string shaderExt;
			if (renderer->getRendererType() == Hollow::RendererType::DirectX) {
				baseShaderPath = "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/";
				shaderExt = ".hlsl";
			} else {
				baseShaderPath = "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/";
				shaderExt = ".glsl";
			}
			{
				Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
				pipelineDesc.vertexShader = shaderManager.create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/gbuffer" + shaderExt, "main" });
				pipelineDesc.pixelShader = shaderManager.create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/gbuffer" + shaderExt, "main" });

				gBufferPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
			}
			{
				Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
				pipelineDesc.vertexShader = shaderManager.create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/depth" + shaderExt, "main" });
				pipelineDesc.pixelShader = shaderManager.create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/depth" + shaderExt, "main" });

				depthPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
			}
			{
				Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
				pipelineDesc.vertexShader = shaderManager.create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/light" + shaderExt, "main" });
				pipelineDesc.pixelShader = shaderManager.create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/light" + shaderExt, "main" });

				lightPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
			}
			{
				Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
				pipelineDesc.vertexShader = shaderManager.create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/SkyMap" + shaderExt, "main" });
				pipelineDesc.pixelShader = shaderManager.create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/SkyMap" + shaderExt, "main" });

				skyMapPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
			}
			{
				Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
				pipelineDesc.vertexShader = shaderManager.create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/terrain" + shaderExt, "main" });
				pipelineDesc.pixelShader = shaderManager.create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/terrain" + shaderExt, "main" });

				terrainPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
			}
			{
				Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
				pipelineDesc.vertexShader = shaderManager.create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/picker" + shaderExt, "main" });
				pipelineDesc.pixelShader = shaderManager.create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/picker" + shaderExt, "main" });

				pickerPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
			}
			{
				Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
				pipelineDesc.vertexShader = shaderManager.create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/flatColor" + shaderExt, "main" });
				pipelineDesc.pixelShader = shaderManager.create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/flatColor" + shaderExt, "main" });

				flatColor = Hollow::ShaderPipeline::create(pipelineDesc);
			}
		}

		// Render targets
		{
			Hollow::RENDER_TARGET_DESC desc;
			desc.count = 3;
			desc.width = this->width;
			desc.height = this->height;
			desc.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R32G32B32A32;
			gBuffer = Hollow::RenderTarget::create(desc);
		}
		{
			Hollow::RENDER_TARGET_DESC desc;
			desc.count = 1;
			desc.width = this->width;
			desc.height = this->height;
			desc.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8;
			picker = Hollow::RenderTarget::create(desc);
		}
		{
			Hollow::RENDER_TARGET_DESC desc;
			desc.count = 1;
			desc.width = 4096;
			desc.height = 4096;
			desc.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8;

			shadow.renderTarget = Hollow::RenderTarget::create(desc);
			shadow.shadowCamera = new Hollow::Camera(false);
			shadow.shadowCamera->setOrthoValues(-1000, 1000, -1000, 1000, -1000, 2000);
			shadow.texelSize = Hollow::Vector2(1.0f / desc.width, 1.0f / desc.height);
			shadow.bias = 0.002f;
		}
		{
			std::vector<Hollow::Vertex> vertices;
			vertices.push_back(Hollow::Vertex(1.0f, 1.0f, 0.0f, 1.0f, 0.0f));
			vertices.push_back(Hollow::Vertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f));
			vertices.push_back(Hollow::Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f));
			vertices.push_back(Hollow::Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f));

			quadVB = Hollow::VertexBuffer::create({ vertices.data(), vertices.size(), sizeof(Hollow::Vertex) });

			unsigned int indices[] = {
				0, 1, 2,
				2, 1, 3
			};

			quadIB = Hollow::IndexBuffer::create({ indices, 6, Hollow::IndexFormat::IFT_UINT });
		}
		{
			std::vector<Hollow::Vertex> vertices;
			vertices.push_back(Hollow::Vertex(0.0f, 10.0f, 0.0f, 0.0f, 0.0f));
			vertices.push_back(Hollow::Vertex(0.0f, -10.0f, 0.0f, 0.0f, 1.0f));

			Hollow::VERTEX_BUFFER_DESC desc;
			desc.data = vertices.data();
			desc.size = vertices.size();
			desc.stride = sizeof(Hollow::Vertex);
			desc.isDynamic = true;

			lineVB = Hollow::VertexBuffer::create(desc);
		}
		std::tie(lightVertexBuffer, lightIndexBuffer) = Hollow::getCube();

		renderer->setViewport(0, 0, this->width, this->height);

		{
			Hollow::DEPTH_STENCIL_STATE_DESC depthDesc;
			depthDesc.depthFunc = Hollow::ComparisonFunction::CMP_LESS;
			less = Hollow::DepthStencil::create(depthDesc);
		}
		{
			Hollow::DEPTH_STENCIL_STATE_DESC depthDesc;
			depthDesc.depthEnable = true;
			depthDesc.depthFunc = Hollow::ComparisonFunction::CMP_GREATER;
			greater = Hollow::DepthStencil::create(depthDesc);
		}
		{
			Hollow::DEPTH_STENCIL_STATE_DESC depthDesc;
			depthDesc.depthEnable = true;
			depthDesc.depthFunc = Hollow::ComparisonFunction::CMP_LESS;

			less = Hollow::DepthStencil::create(depthDesc);
		}

		{
			Hollow::RASTERIZER_STATE_DESC desc;
			desc.cullMode = Hollow::CullMode::CLM_BACK;
			cullBack = Hollow::RasterizerState::create(desc);
		}
		{
			Hollow::RASTERIZER_STATE_DESC desc;
			desc.cullMode = Hollow::CullMode::CLM_FRONT;
			cullFront = Hollow::RasterizerState::create(desc);
		}
		{
			Hollow::RASTERIZER_STATE_DESC desc;
			desc.cullMode = Hollow::CullMode::CLM_NONE;
			desc.fillMode = Hollow::FillMode::FM_SOLID;
			cullBackWF = Hollow::RasterizerState::create(desc);
		}
		{
			Hollow::RASTERIZER_STATE_DESC desc;
			desc.cullMode = Hollow::CullMode::CLM_NONE;
			cullNone = Hollow::RasterizerState::create(desc);
		}
		{
			Hollow::SAMPLER_STATE_DESC desc;
			desc.magFilterMode = Hollow::FilterMode::FM_LINEAR;
			desc.minFilterModel = Hollow::FilterMode::FM_LINEAR;
			desc.mipFilterMode = Hollow::FilterMode::FM_POINT;

			desc.comparisonFunction = Hollow::ComparisonFunction::CMP_NEVER;
			sampler = Hollow::RenderStateManager::instance()->createSamplerState(desc);
		}
		{
			Hollow::SAMPLER_STATE_DESC desc;
			desc.magFilterMode = Hollow::FilterMode::FM_LINEAR;
			desc.minFilterModel = Hollow::FilterMode::FM_LINEAR;
			desc.mipFilterMode = Hollow::FilterMode::FM_NONE;
			desc.addressingMode = Hollow::AddressingMode::AM_CLAMP;

			desc.comparisonFunction = Hollow::ComparisonFunction::CMP_NEVER;
			renderTargetSampler = Hollow::RenderStateManager::instance()->createSamplerState(desc);
		}
		
		defaultMaterial = new Hollow::Material();
		defaultMaterial->materialData.color = Hollow::Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		renderer->setSampler(0, sampler);
		renderer->setSampler(1, sampler);

		for (int i = 0; i < 10; i++) {
			renderer->setTextureSampler(i, sampler);
		}

		timer.start();
	}

	virtual void PreUpdate(double dt)
	{
		renderer->clearRenderTarget(0, (float*)ClearColor);
		renderer->clearRenderTarget(gBuffer, ClearColor);
		renderer->clearRenderTarget(shadow.renderTarget, ClearColor);
		renderer->clearRenderTarget(picker, ClearColor);
	}

	virtual void Update(double dt)
	{
		Profiler::begin("RenderSystem update(): ");
		viewProjection = m_Camera->getProjectionMatrix() * m_Camera->getViewMatrix();

		culled = 0;
		renderer->setInputLayout(defaultLayout);
		shadow.shadowCamera->update(dt);

		updateWVP(this->m_Camera);
		// GBuffer pass
		timer.restart();
		for (int i = 0; i < 5; i++) {
			renderer->setTextureSampler(i, sampler);
		}
		gBufferPass();
		gbufferTime = timer.getMilisecondsElapsed();

		// Picker pass
		timer.restart();
		renderer->setDepthStencilState(less);
		pickerPass();
		pickerTime = timer.getMilisecondsElapsed();

		// Shadow pass
		timer.restart();
		shadowPass();
		shadowTime = timer.getMilisecondsElapsed();

		// Light pass
		{
			renderer->setTextureSampler(0, renderTargetSampler);
			renderer->setTextureSampler(1, renderTargetSampler);
			renderer->setTextureSampler(3, renderTargetSampler);
			renderer->setTextureSampler(4, renderTargetSampler);
			renderer->setTextureSampler(5, renderTargetSampler);

			renderer->setRasterizerState(cullBack);
			renderer->setRenderTarget(0);
			renderer->setTextureColorBuffer(0, gBuffer, 0);
			renderer->setTextureColorBuffer(1, gBuffer, 1);
			renderer->setTextureColorBuffer(2, gBuffer, 2);
			renderer->setTextureDepthBuffer(3, shadow.renderTarget);
			renderer->setTextureDepthBuffer(5, gBuffer);

			renderer->setShaderPipeline(lightPipeline);
			updateLight();

			renderer->setVertexBuffer(quadVB);
			renderer->setIndexBuffer(quadIB);
			renderer->drawIndexed(6);

			renderer->setDepthStencilState(greater);
			renderer->setRasterizerState(cullFront);
			DrawSkyMap();

			renderer->unsetTexture(0);
			renderer->unsetTexture(1);
			renderer->unsetTexture(2);
			renderer->unsetTexture(3);
			renderer->unsetTexture(5);
		}

		lightTime = timer.getMilisecondsElapsed();;

		if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_CONTROL)
			&& Hollow::InputManager::GetMouseButtonIsPressed(Hollow::eMouseKeyCodes::MOUSE_LEFT)
		) {
			Hollow::Vector4 selectedColor = picker->readPixel(Hollow::InputManager::mcx, Hollow::InputManager::mcy);
			pickedID = selectedColor.x + selectedColor.y * 256 + selectedColor.z * 256 * 256;
			Hollow::EventSystem::instance()->addEvent(new ChangeSelectedEntity(pickedID));
		}
		Profiler::end();
	}

	virtual void PostUpdate(double dt)
	{
		renderer->present();
	}

	// Update world view projection matrix
	void updateWVP(Hollow::Camera* camera)
	{
		m_wvp.WVP = camera->getProjectionMatrix() * camera->getViewMatrix();
		m_wvp.cameraPosition = camera->getPositionVec3();
		m_wvp.cameraViewDirection = camera->getRotationVec3();
		m_WVPConstantBuffer->update(&m_wvp);
		renderer->setGpuBuffer(m_WVPConstantBuffer);
	}

	void DrawSkyMap()
	{
		Hollow::Matrix4 viewMatrx = m_Camera->getViewMatrix();
		viewMatrx.r[0].w = 0.0f;
		viewMatrx.r[1].w = 0.0f;
		viewMatrx.r[2].w = 0.0f;
		m_wvp.WVP = m_Camera->getProjectionMatrix() * viewMatrx;

		m_WVPConstantBuffer->update(&m_wvp);
		renderer->setGpuBuffer(m_WVPConstantBuffer);

		renderer->setTexture(4, skyMap->material->diffuseTexture);
		renderer->setShaderPipeline(skyMapPipeline);

		renderer->setVertexBuffer(skyMap->vBuffer);
		renderer->setIndexBuffer(skyMap->iBuffer);
		renderer->drawIndexed(skyMap->iBuffer->mHardwareBuffer->getSize());
	}

	void gBufferPass()
	{
		renderer->setDepthStencilState(less);
		renderer->setRasterizerState(cullBackWF);
		renderer->setRenderTarget(gBuffer);
		renderer->setShaderPipeline(gBufferPipeline);

		// Physics debug draw
		{
			renderer->setPrimitiveTopology(Hollow::PrimitiveTopology::PT_LINELIST);
			defaultMaterial->materialData.color.x = 1.0f;
			defaultMaterial->materialData.color.y = 0.0f;
			defaultMaterial->materialData.color.z = 1.0f;
			defaultMaterial->materialData.color.w = 1.0f;
			materialConstantBuffer->update(&defaultMaterial->materialData);
			defaultMaterial->materialData.color.x = 0.0f;
			defaultMaterial->materialData.color.y = 1.0f;
			defaultMaterial->materialData.color.z = 0.0f;
			renderer->setGpuBuffer(materialConstantBuffer);

			perModelData.transform = Hollow::Matrix4::identity();
			perModel->update(&perModelData);
			renderer->setGpuBuffer(perModel);

			PhysicsSystem::instance()->dynamicsWorld->debugDrawWorld();

			renderer->setPrimitiveTopology(Hollow::PrimitiveTopology::PT_TRIANGELIST);
		}

		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				if (entity.hasComponent<AnimationComponent>()) {
					AnimationComponent* animation = entity.getComponent<AnimationComponent>();
					perModelData.hasAnimation = !animation->stoped;
					if (!animation->stoped) {
						AnimationComponent* animation = entity.getComponent<AnimationComponent>();
						boneInfo->update(animation->nodeInfo.data(), sizeof(Hollow::Matrix4) * animation->nodeInfo.size());
						renderer->setGpuBuffer(boneInfo);
					}
				} else {
					perModelData.hasAnimation = false;
				}

				Hollow::Matrix4 trs = Hollow::Matrix4::translation(transform->position) 
					* Hollow::Matrix4::rotation(Hollow::Quaternion(transform->rotation))
					* Hollow::Matrix4::scaling(transform->scale);

				perModelData.transform = trs;
				perModel->update(&perModelData);
				renderer->setGpuBuffer(perModel);

				for (auto& object : renderable->renderables)  {
					if (renderable->materials.find(object->material) != renderable->materials.end()) {
						Hollow::Material* material = renderable->materials[object->material];
						if (material->diffuseTexture != nullptr) {
							renderer->setTexture(0, material->diffuseTexture);
						} else {
							renderer->unsetTexture(0);
						}
						materialConstantBuffer->update(&renderable->materials[object->material]->materialData);
						renderer->setGpuBuffer(materialConstantBuffer);
					} else {
						materialConstantBuffer->update(&defaultMaterial->materialData);
						renderer->setGpuBuffer(materialConstantBuffer);
					}
					

					renderer->setVertexBuffer(object->vBuffer);
					renderer->setIndexBuffer(object->iBuffer);
					renderer->drawIndexed(object->iBuffer->mHardwareBuffer->getSize());
				}
			}
		}

		for (auto& entity : Hollow::EntityManager::instance()->container<Light>()) {
			if (entity.hasComponent<LightComponent>()) {
				LightComponent* lightComponent = entity.getComponent<LightComponent>();

				perModelData.transform = Hollow::Matrix4::transpose(Hollow::Matrix4::translation(lightComponent->lightData.position));
				perModel->update(&perModelData);
				renderer->setGpuBuffer(perModel);

				renderer->setVertexBuffer(lightVertexBuffer);
				renderer->setIndexBuffer(lightIndexBuffer);
				renderer->drawIndexed(lightIndexBuffer->mHardwareBuffer->getSize());
			}
		}

		renderer->setInputLayout(terrainLayout);
		renderer->setShaderPipeline(terrainPipeline);
		for (auto& entity : Hollow::EntityManager::instance()->container<Terrain>()) {
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<TerrainData>()) {
				TerrainData* data = entity.getComponent<TerrainData>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				if (data->vBuffer != nullptr) {
					perModelData.transform = Hollow::Matrix4::translation(transform->position)
						* Hollow::Matrix4::rotation(Hollow::Quaternion(transform->rotation))
						* Hollow::Matrix4::scaling(transform->scale);

					perModel->update(&perModelData);
					renderer->setGpuBuffer(perModel);
					renderer->setTexture(0, data->tex);
					renderer->setVertexBuffer(data->vBuffer);
					renderer->draw(data->vBuffer->mHardwareBuffer->getSize());
				}
			}
		}
		renderer->setInputLayout(defaultLayout);
		renderer->setRasterizerState(cullBack);
	}

	void shadowPass()
	{
		renderer->setViewport(0, 0, 4096, 4096);
		renderer->setRasterizerState(cullFront);

		shadowStruct.ShadowWVP = shadow.shadowCamera->getProjectionMatrix() * shadow.shadowCamera->getViewMatrix();
		shadowStruct.texelSize = shadow.texelSize;
		shadowStruct.bias = shadow.bias;
		shadowConstantBuffer->update(&shadowStruct);
		renderer->setGpuBuffer(shadowConstantBuffer);

		renderer->setRenderTarget(shadow.renderTarget);
		renderer->setShaderPipeline(depthPipeline);

		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				perModelData.transform = Hollow::Matrix4::translation(transform->position)
					* Hollow::Matrix4::rotation(Hollow::Quaternion(transform->rotation))
					* Hollow::Matrix4::scaling(transform->scale);

				perModel->update(&perModelData);
				renderer->setGpuBuffer(perModel);

				for (auto& object : renderable->renderables) {
					renderer->setVertexBuffer(object->vBuffer);
					renderer->setIndexBuffer(object->iBuffer);
					renderer->drawIndexed(object->iBuffer->mHardwareBuffer->getSize());
				}
			}
		}
		for (auto& entity : Hollow::EntityManager::instance()->container<Terrain>()) {
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<TerrainData>()) {
				TerrainData* data = entity.getComponent<TerrainData>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				if (data->vBuffer != nullptr) {
					perModelData.transform = Hollow::Matrix4::translation(transform->position)
						* Hollow::Matrix4::rotation(Hollow::Quaternion(transform->rotation))
						* Hollow::Matrix4::scaling(transform->scale);

					perModel->update(&perModelData);
					renderer->setGpuBuffer(perModel);
					renderer->setTexture(0, data->tex);
					renderer->setVertexBuffer(data->vBuffer);
					renderer->draw(data->vBuffer->mHardwareBuffer->getSize());
				}
			}
		}
		renderer->setViewport(0, 0, width, height);
	}

	void pickerPass()
	{
		renderer->setRenderTarget(picker);
		renderer->setShaderPipeline(pickerPipeline);
		renderer->setRasterizerState(cullBack);

		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				perModelData.transform = Hollow::Matrix4::translation(transform->position)
					* Hollow::Matrix4::rotation(Hollow::Quaternion(transform->rotation))
					* Hollow::Matrix4::scaling(transform->scale);

				perModelData.color = Hollow::Vector3(
					((entity.getId() & 0x000000FF) >> 0) / 255.0f, 
					((entity.getId() & 0x0000FF00) >> 8) / 255.0f, 
					((entity.getId() & 0x00FF0000) >> 16) / 255.0f
				);

				perModel->update(&perModelData);
				renderer->setGpuBuffer(perModel);

				for (auto& object : renderable->renderables) {
					renderer->setVertexBuffer(object->vBuffer);
					renderer->setIndexBuffer(object->iBuffer);
					renderer->drawIndexed(object->iBuffer->mHardwareBuffer->getSize());
				}
			}
		}
	}

	void updateLight()
	{
		int counter = 0;
		memset(&lightInfo, 0, sizeof(LightInfo));
		for (auto& entity : Hollow::EntityManager::instance()->container<Light>()) {
			if (entity.hasComponent<LightComponent>()) {
				LightComponent* lightComponent = entity.getComponent<LightComponent>();
				lightInfo.lightData[counter++] = lightComponent->lightData;
			}
		}
		lightInfo.numLights = counter;
		lightInfoBuffer->update(&lightInfo);
		renderer->setGpuBuffer(lightInfoBuffer);
	}
};
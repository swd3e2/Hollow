#pragma once
#include "Sandbox/Components/TransformComponent.h"
#include "Hollow/ECS/System.h"
#include "Hollow/ECS/EntityManager.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Entities/Light.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Resources/Mesh/Mesh.h"
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
#include "Sandbox/Components/ParticleComponent.h"
#include "Hollow/Events/EventSystem.h"
#include "Hollow/Events/DefaultEvents.h"
#include <array>

struct WVP
{
	Hollow::Matrix4 WVP;
	Hollow::Vector3 cameraPosition;
	Hollow::Vector3 cameraViewDirection;
	Hollow::Matrix4 ViewMatrix;
	Hollow::Matrix4 ProjectionMatrix;
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
	Hollow::Matrix4 worldTransform;
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

struct ParticleData
{
	int maxOffset;
	int currentOffset;
	Hollow::Vector2 texCoords;
};

class RenderSystem : public Hollow::System<RenderSystem>, public Hollow::IEventListener
{
public:
	Shadow shadow;
	Hollow::Camera* m_Camera;
	SkyMap* skyMap;
	Hollow::s_ptr<Hollow::RenderTarget> main;
	Hollow::s_ptr<Hollow::RenderTarget> gBuffer;
	Hollow::s_ptr<Hollow::RenderTarget> picker;
	Hollow::s_ptr<Hollow::RenderTarget> light;

	int pickedID;
	int culled = 0;
	double gbufferTime;
	double shadowTime;
	double pickerTime;
	double lightTime;
	Hollow::s_ptr<Hollow::VertexBuffer> lineVB;

	bool enableDebugPhysicsDraw = false;
private:
	Hollow::RenderApi* renderer;
	const std::string defaultShaderPath = "E:/Hollow/Hollow/Hollow/Data/Shaders/OGL/";
private:
	Hollow::Timer timer;
	WVP m_wvp;

	Hollow::s_ptr<Hollow::InputLayout> defaultLayout;
	Hollow::s_ptr<Hollow::InputLayout> terrainLayout;

	ShadowStruct shadowStruct;

	Hollow::s_ptr<Hollow::GPUBuffer> shadowConstantBuffer;
	Hollow::s_ptr<Hollow::GPUBuffer> m_WVPConstantBuffer;
	Hollow::s_ptr<Hollow::GPUBuffer> materialConstantBuffer;
	Hollow::s_ptr<Hollow::GPUBuffer> lightInfoBuffer;
	Hollow::s_ptr<Hollow::GPUBuffer> boneInfo;
	Hollow::s_ptr<Hollow::GPUBuffer> instanceData;
	Hollow::s_ptr<Hollow::GPUBuffer> perModel;
	Hollow::s_ptr<Hollow::GPUBuffer> perMesh;
	Hollow::s_ptr<Hollow::GPUBuffer> particleDataConstantBuffer;

	PerModel perModelData;
	PerMesh perMeshData;
	ParticleData particleData;

	Hollow::s_ptr<Hollow::ShaderPipeline> gBufferPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> depthPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> lightPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> skyMapPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> defaultPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> terrainPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> pickerPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> flatColor;
	Hollow::s_ptr<Hollow::ShaderPipeline> instanced;
	Hollow::s_ptr<Hollow::ShaderPipeline> samplingPipeline;
	Hollow::s_ptr<Hollow::ShaderPipeline> geomtryShaderTest;
	Hollow::s_ptr<Hollow::ShaderPipeline> pbr;
	Hollow::s_ptr<Hollow::ShaderPipeline> particlePipeline;

	int pointLightsNum = 0;
	int directionalLightNum = 0;
	int spotLifhtNum = 0;

	int width;
	int height;

	const UINT uavs = 0;

	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	Hollow::s_ptr<Hollow::VertexBuffer> quadVB;
	Hollow::s_ptr<Hollow::IndexBuffer>	quadIB;

	Hollow::Vector4 AABBplane[6];
	Hollow::s_ptr<Hollow::VertexBuffer> lightVertexBuffer;
	Hollow::s_ptr<Hollow::IndexBuffer> lightIndexBuffer;

	Hollow::Material lightMaterial;
	LightInfo lightInfo;
	Hollow::Matrix4 viewProjection;

	Hollow::s_ptr<Hollow::DepthStencil> less;
	Hollow::s_ptr<Hollow::DepthStencil> greater;
	Hollow::s_ptr<Hollow::DepthStencil> noDepth;

	Hollow::s_ptr<Hollow::RasterizerState> cullBack;
	Hollow::s_ptr<Hollow::RasterizerState> cullFront;
	Hollow::s_ptr<Hollow::RasterizerState> cullNone;

	Hollow::s_ptr<Hollow::SamplerState> sampler;
	Hollow::s_ptr<Hollow::SamplerState> renderTargetSampler;
	Hollow::Material* defaultMaterial;

	std::array<Hollow::Matrix4, 50> instanceGpuData;

	Hollow::s_ptr<Hollow::VertexBuffer> geometryShaderTestBuffer;
public:
	RenderSystem(Hollow::RenderApi* renderer, int width, int height) :
		renderer(renderer), width(width), height(height)
	{
		initGpuBuffers();
		initInputLayout();
		initShaders();
		initRenderTarget();
		initDepthStencilState();
		initRasterizerState();
		initSamplerState();
		
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
			vertices.push_back(Hollow::Vertex(0.0f, 10.0f, 0.0f));
			vertices.push_back(Hollow::Vertex(0.0f, -10.0f, 0.0f));

			Hollow::VERTEX_BUFFER_DESC desc;
			desc.data = vertices.data();
			desc.size = vertices.size();
			desc.stride = sizeof(Hollow::Vertex);
			desc.isDynamic = true;

			lineVB = Hollow::VertexBuffer::create(desc);
		}
		{
			std::vector<Hollow::Vertex> vertices;
			vertices.push_back(Hollow::Vertex( 0.5f,  0.5f, 0.0f));
			vertices.push_back(Hollow::Vertex( 0.5f, -0.5f, 0.0f));
			vertices.push_back(Hollow::Vertex(-0.5f,  0.5f, 0.0f));
			vertices.push_back(Hollow::Vertex(-0.5f, -0.5f, 0.0f));

			Hollow::VERTEX_BUFFER_DESC desc;
			desc.data = vertices.data();
			desc.size = vertices.size();
			desc.stride = sizeof(Hollow::Vertex);

			geometryShaderTestBuffer = Hollow::VertexBuffer::create(desc);
		}
		std::tie(lightVertexBuffer, lightIndexBuffer) = Hollow::getCube();

		renderer->setViewport(0, 0, this->width, this->height);

		defaultMaterial = new Hollow::Material();
		defaultMaterial->materialData.color = Hollow::Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		renderer->setSampler(0, sampler);
		renderer->setSampler(1, sampler);

		for (int i = 0; i < 10; i++) {
			renderer->setTextureSampler(i, sampler);
		}

		for (int i = 0; i < 50; i++) {
			instanceGpuData[i] = Hollow::Matrix4::translation(((i + 1) % 10) * 100, 0, ((i + 1) / 10) * 100);
		}

		instanceData->update(instanceGpuData.data());
		renderer->setGpuBuffer(instanceData);

		renderer->setPrimitiveTopology(Hollow::PrimitiveTopology::PT_TRIANGELIST);

		timer.start();

		Hollow::EventSystem::instance()->addEventListener(this, &RenderSystem::onWindowResize, Hollow::WindowResizeEvent::staticGetId());
	}

	void onWindowResize(Hollow::IEvent* event)
	{
		Hollow::WindowResizeEvent windowResizeEvent = *(Hollow::WindowResizeEvent*)event;
		Hollow::DelayedTaskManager::instance()->add([&, windowResizeEvent]() {
			renderer->setViewport(0, 0, windowResizeEvent.width, windowResizeEvent.height);
		});
	}

	virtual void PreUpdate(double dt)
	{
		renderer->clearRenderTarget(0, (float*)ClearColor);
		renderer->clearRenderTarget(gBuffer, ClearColor);
		renderer->clearRenderTarget(light, ClearColor);
		renderer->clearRenderTarget(shadow.renderTarget, ClearColor);
		renderer->clearRenderTarget(picker, ClearColor);
	}

	virtual void Update(double dt)
	{
		Profiler::begin("RenderSystem update(): ");
		viewProjection = m_Camera->getProjectionMatrix() * m_Camera->getViewMatrix();

		culled = 0;
		renderer->setInputLayout(defaultLayout);

		updateWVP(this->m_Camera);
		// GBuffer pass
		for (int i = 0; i < 5; i++) {
			renderer->setTextureSampler(i, sampler);
		}

		timer.restart();
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
			renderer->setRenderTarget(light);
			renderer->setTextureColorBuffer(0, gBuffer, 0);
			renderer->setTextureColorBuffer(1, gBuffer, 1);
			renderer->setTextureColorBuffer(2, gBuffer, 2);
			renderer->setTextureColorBuffer(3, gBuffer, 3);
			renderer->setTextureDepthBuffer(4, shadow.renderTarget);
			renderer->setTextureDepthBuffer(5, gBuffer);

			renderer->setShaderPipeline(lightPipeline);
			updateLight();

			renderer->setVertexBuffer(quadVB);
			renderer->setIndexBuffer(quadIB);
			renderer->drawIndexed(6);

			renderer->setDepthStencilState(greater);
			renderer->setRasterizerState(cullFront);
			//DrawSkyMap();

			renderer->unsetTexture(0);
			renderer->unsetTexture(1);
			renderer->unsetTexture(2);
			renderer->unsetTexture(3);
			renderer->unsetTexture(5);
		}
		{
			renderer->setRenderTarget(0);
			renderer->setDepthStencilState(less);
			renderer->setRasterizerState(cullNone);
			renderer->setShaderPipeline(samplingPipeline);
			renderer->setTextureColorBuffer(0, light, 0);
			renderer->setVertexBuffer(quadVB);
			renderer->setIndexBuffer(quadIB);
			renderer->drawIndexed(6);
			renderer->unsetTexture(0);
		}
		lightTime = timer.getMilisecondsElapsed();;

		
		if (Hollow::InputManager::GetMouseButtonIsPressed(Hollow::eMouseKeyCodes::MOUSE_LEFT) &&
			Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_CONTROL)
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
		m_wvp.ViewMatrix = camera->getViewMatrix();
		m_wvp.ProjectionMatrix = camera->getProjectionMatrix();

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
		renderer->setRasterizerState(cullNone);
		renderer->setRenderTarget(gBuffer);
		renderer->setDepthStencilState(noDepth);
		renderer->setShaderPipeline(gBufferPipeline);

		// Physics debug draw
		if (enableDebugPhysicsDraw) {
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

			perMeshData.worldTransform = Hollow::Matrix4::identity();
			perMesh->update(&perMeshData);
			renderer->setGpuBuffer(perMesh);
			perModelData.transform = Hollow::Matrix4::identity();
			perModelData.hasAnimation = false;
			perModel->update(&perModelData);
			renderer->setGpuBuffer(perModel);

			PhysicsSystem::instance()->dynamicsWorld->debugDrawWorld();

			renderer->setPrimitiveTopology(Hollow::PrimitiveTopology::PT_TRIANGELIST);
		}

		renderer->setDepthStencilState(less);

		drawModels();
		drawLights();
		drawParticles();

		//renderer->setPrimitiveTopology(Hollow::PrimitiveTopology::PT_POINT);
		//renderer->setShaderPipeline(geomtryShaderTest);
		//renderer->setVertexBuffer(geometryShaderTestBuffer);
		//renderer->draw(4);
		//renderer->setPrimitiveTopology(Hollow::PrimitiveTopology::PT_TRIANGELIST);

		renderer->setRasterizerState(cullBack);
	}

	void drawParticles()
	{
		perMeshData.worldTransform = Hollow::Matrix4::identity();
		perMesh->update(&perMeshData);

		renderer->setShaderPipeline(particlePipeline);

		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<ParticleComponent>()) {
				ParticleComponent* particle = entity.getComponent<ParticleComponent>();
				renderer->setTexture(0, particle->texture);

				for (auto& it : particle->particles) {
					perModelData.transform = Hollow::Matrix4::translation(it->position) 
						* Hollow::Matrix4::scaling(it->scale);
					perModel->update(&perModelData);

					particleData.currentOffset = it->currentOffset;
					particleData.maxOffset = particle->maxOffsets;
					particleData.texCoords = particle->texCoords;
					particleDataConstantBuffer->update(&particleData);
					renderer->setGpuBuffer(particleDataConstantBuffer);

					renderer->setGpuBuffer(perModel);

					renderer->setVertexBuffer(quadVB);
					renderer->setIndexBuffer(quadIB);
					renderer->drawIndexed(lightIndexBuffer->mHardwareBuffer->getSize());
				}
			}
		}
	}

	void drawLights()
	{
		for (auto& entity : Hollow::EntityManager::instance()->container<Light>()) {
			if (entity.hasComponent<LightComponent>()) {
				LightComponent* lightComponent = entity.getComponent<LightComponent>();

				if (lightComponent->lightData.type == (int)LightType::POINT) {
					perModelData.transform = Hollow::Matrix4::translation(lightComponent->lightData.position);
					perModelData.hasAnimation = false;
					perModel->update(&perModelData);
					renderer->setGpuBuffer(perModel);

					renderer->setVertexBuffer(lightVertexBuffer);
					renderer->setIndexBuffer(lightIndexBuffer);
					renderer->drawIndexed(lightIndexBuffer->mHardwareBuffer->getSize());
				}
			}
		}
	}

	void drawModels()
	{
		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) {
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				if (renderable->skinned && entity.hasComponent<AnimationComponent>()) {
					AnimationComponent* animation = entity.getComponent<AnimationComponent>();
					perModelData.hasAnimation = !animation->stoped;
					if (!animation->stoped && animation->nodeInfo.size() > 0) {
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

				if (false) {
					for (auto& object : renderable->renderables) {
						if (renderable->materials.size() > object->material) {
							const Hollow::s_ptr<Hollow::Material>& material = renderable->materials[object->material];
							if (material->diffuseTexture != nullptr) {
								renderer->setTexture(0, material->diffuseTexture);
							}
							else {
								renderer->unsetTexture(0);
							}
							materialConstantBuffer->update(&renderable->materials[object->material]->materialData);
							renderer->setGpuBuffer(materialConstantBuffer);
						}
						else {
							materialConstantBuffer->update(&defaultMaterial->materialData);
							renderer->setGpuBuffer(materialConstantBuffer);
						}


						renderer->setVertexBuffer(object->vBuffer);
						renderer->setIndexBuffer(object->iBuffer);
						renderer->drawIndexed(object->iBuffer->mHardwareBuffer->getSize());
					}
				} else if (renderable->rootNode != -1) {
					drawNodeHierarchy(renderable->rootNode, renderable);
				}
			}
		}
	}

	void drawNodeHierarchy(const int nodeId, RenderableComponent* renderable)
	{
		if (renderable->nodes.size() <= nodeId) return;

		const Hollow::s_ptr<RenderableComponent::Node>& node = renderable->nodes[nodeId];
		if (node->mesh != -1 && node->mesh < renderable->renderables.size()) {
			const Hollow::s_ptr<RenderableComponent::Mesh>& mesh = renderable->renderables[node->mesh];

			if (renderable->materials.size() > mesh->material) {
				const Hollow::s_ptr<Hollow::Material>& material = renderable->materials[mesh->material];
				if (material->diffuseTexture != nullptr) {
					renderer->setTexture(0, material->diffuseTexture);
				} else {
					renderer->unsetTexture(0);
				}
				if (material->normalTexture != nullptr) {
					renderer->setTexture(1, material->normalTexture);
				} else {
					renderer->unsetTexture(1);
				}
				materialConstantBuffer->update(&renderable->materials[mesh->material]->materialData);
				renderer->setGpuBuffer(materialConstantBuffer);
			} else {
				materialConstantBuffer->update(&defaultMaterial->materialData);
				renderer->setGpuBuffer(materialConstantBuffer);
			}

			perMeshData.worldTransform = node->transform.worldTransform;
			perMesh->update(&perMeshData);
			renderer->setGpuBuffer(perMesh);

			renderer->setVertexBuffer(mesh->vBuffer);
			renderer->setIndexBuffer(mesh->iBuffer);
			renderer->drawIndexed(mesh->iBuffer->mHardwareBuffer->getSize());
		}

		for (auto& it : node->childs) {
			drawNodeHierarchy(it, renderable);
		}
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

	void initShaders()
	{
		std::string baseShaderPath;
		std::string shaderExt;
		if (renderer->getRendererType() == Hollow::RendererType::DirectX) {
			baseShaderPath = "E:/Hollow/Hollow/Hollow/Data/Shaders/D3D11/";
			shaderExt = ".hlsl";
		} else {
			baseShaderPath = "E:/Hollow/Hollow/Hollow/Data/Shaders/OGL/";
			shaderExt = ".glsl";
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/gbuffer" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/gbuffer" + shaderExt, "main" });

			gBufferPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/depth" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/depth" + shaderExt, "main" });

			depthPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/light" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/light" + shaderExt, "main" });

			lightPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/SkyMap" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/SkyMap" + shaderExt, "main" });

			skyMapPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/terrain" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/terrain" + shaderExt, "main" });

			terrainPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/picker" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/picker" + shaderExt, "main" });

			pickerPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/flatColor" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/flatColor" + shaderExt, "main" });

			flatColor = Hollow::ShaderPipeline::create(pipelineDesc);
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/instanced" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/instanced" + shaderExt, "main" });

			instanced = Hollow::ShaderPipeline::create(pipelineDesc);
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/sampling" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/sampling" + shaderExt, "main" });

			samplingPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/pbr" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/pbr" + shaderExt, "main" });

			pbr = Hollow::ShaderPipeline::create(pipelineDesc);
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, baseShaderPath + "vertex/geomtryShaderTest" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, baseShaderPath + "pixel/geomtryShaderTest" + shaderExt, "main" });
			pipelineDesc.geometryShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_GEOMERTY, baseShaderPath + "geometry/geomtryShaderTest" + shaderExt, "main" });

			geomtryShaderTest = Hollow::ShaderPipeline::create(pipelineDesc);
		}
		{
			Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
			pipelineDesc.vertexShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_VERTEX, defaultShaderPath + "particleVs" + shaderExt, "main" });
			pipelineDesc.pixelShader = ShaderManager::instance()->create({ Hollow::ShaderType::ST_PIXEL, defaultShaderPath + "particlePS" + shaderExt, "main" });

			particlePipeline = Hollow::ShaderPipeline::create(pipelineDesc);
		}
	}

	void initGpuBuffers()
	{
		m_WVPConstantBuffer = Hollow::GPUBuffer::create(0, sizeof(WVP));
		shadowConstantBuffer = Hollow::GPUBuffer::create(1, sizeof(ShadowStruct));
		perModel = Hollow::GPUBuffer::create(2, sizeof(PerModel));
		perMesh = Hollow::GPUBuffer::create(3, sizeof(PerMesh));
		materialConstantBuffer = Hollow::GPUBuffer::create(4, sizeof(Hollow::MaterialData));
		lightInfoBuffer = Hollow::GPUBuffer::create(5, sizeof(LightInfo));
		boneInfo = Hollow::GPUBuffer::create(6, sizeof(Hollow::Matrix4) * 200);
		instanceData = Hollow::GPUBuffer::create(7, sizeof(Hollow::Matrix4) * 50);
		particleDataConstantBuffer = Hollow::GPUBuffer::create(3, sizeof(ParticleData));
	}

	void initInputLayout()
	{
		Hollow::INPUT_LAYOUT_DESC layoutDesc = {
			{ Hollow::INPUT_DATA_TYPE::Float3, "POSITION"	},
			{ Hollow::INPUT_DATA_TYPE::Float2, "TEXCOORD"	},
			{ Hollow::INPUT_DATA_TYPE::Float3, "NORMAL"		},
			{ Hollow::INPUT_DATA_TYPE::Float3, "TANGENT"	},
			{ Hollow::INPUT_DATA_TYPE::Float3, "BITANGENT"	}, 
			{ Hollow::INPUT_DATA_TYPE::Int4,   "BONE"		},
			{ Hollow::INPUT_DATA_TYPE::Float4, "WEIGHT"		}
		};

		defaultLayout = Hollow::InputLayout::create(layoutDesc);

		Hollow::INPUT_LAYOUT_DESC terrainLayoutDesc = {
			{ Hollow::INPUT_DATA_TYPE::Float3, "POSITION"	},
			{ Hollow::INPUT_DATA_TYPE::Float2, "TEXCOORD"	},
			{ Hollow::INPUT_DATA_TYPE::Float3, "NORMAL"		},	
			{ Hollow::INPUT_DATA_TYPE::Float3, "COLOR"		}
		};

		terrainLayout = Hollow::InputLayout::create(terrainLayoutDesc);
	}

	void initRenderTarget()
	{
		{
			Hollow::RENDER_TARGET_DESC desc;
			desc.count = 4;
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
			desc.width = this->width;
			desc.height = this->height;
			desc.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8;
			light = Hollow::RenderTarget::create(desc);
		}
		{
			Hollow::RENDER_TARGET_DESC desc;
			desc.count = 1;
			desc.width = 4096;
			desc.height = 4096;
			desc.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8;

			shadow.renderTarget = Hollow::RenderTarget::create(desc);
			shadow.shadowCamera = new Hollow::Camera();
			shadow.shadowCamera->setOrthoValues(-1000, 1000, -1000, 1000, -1000, 2000);
			shadow.texelSize = Hollow::Vector2(1.0f / desc.width, 1.0f / desc.height);
			shadow.bias = 0.002f;
		}
	}

	void initDepthStencilState()
	{
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
			depthDesc.depthFunc = Hollow::ComparisonFunction::CMP_ALWAYS;
			noDepth = Hollow::DepthStencil::create(depthDesc);
		}
	}

	void initRasterizerState()
	{
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
			cullNone = Hollow::RasterizerState::create(desc);
		}
	}

	void initSamplerState()
	{
		{
			Hollow::SAMPLER_STATE_DESC desc;
			desc.magFilterMode = Hollow::FilterMode::FM_ANISOTROPIC;
			desc.minFilterModel = Hollow::FilterMode::FM_ANISOTROPIC;
			desc.mipFilterMode = Hollow::FilterMode::FM_ANISOTROPIC;
			desc.maxAnisotropy = 16;

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
	}
};
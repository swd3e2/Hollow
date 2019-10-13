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

using namespace Hollow;

struct WVP
{
	Matrix4 WVP;
	Vector3 cameraPosition;
	Vector3 cameraViewDirection;
};

struct PerModel
{
	Matrix4 transform;
	Vector3 color;
	int selected;
	int hasAnimation;
};

struct PerMesh
{
	Vector3 color;
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
	LightData lightData[100];
	int numLights;
};

class RenderSystem : public Hollow::System<RenderSystem>
{
public:
	Shadow shadow;
	Camera* m_Camera;
	SkyMap* skyMap;
	s_ptr<RenderTarget> main;
	s_ptr<RenderTarget> gBuffer;
	s_ptr<RenderTarget> picker;

	int pickedID;
	int culled = 0;
	double gbufferTime;
	double shadowTime;
	double pickerTime;
	double lightTime;
private:
	RenderApi* renderer;
private:
	Hollow::Timer timer;
	WVP						m_wvp;

	s_ptr<Hollow::InputLayout> defaultLayout;
	s_ptr<Hollow::InputLayout> terrainLayout;

	ShadowStruct shadowStruct;

	s_ptr<GPUBuffer> shadowConstantBuffer;
	s_ptr<GPUBuffer> m_WVPConstantBuffer;
	s_ptr<GPUBuffer> m_WorldViewProjectionBuffer;
	s_ptr<GPUBuffer> materialConstantBuffer;
	s_ptr<GPUBuffer> lightInfoBuffer;
	s_ptr<GPUBuffer> boneInfo;

	s_ptr<GPUBuffer> perModel;
	s_ptr<GPUBuffer> perMesh;
	PerModel perModelData;
	PerMesh perMeshData;

	s_ptr<ShaderPipeline> gBufferPipeline;
	s_ptr<ShaderPipeline> depthPipeline;
	s_ptr<ShaderPipeline> lightPipeline;
	s_ptr<ShaderPipeline> skyMapPipeline;
	s_ptr<ShaderPipeline> defaultPipeline;
	s_ptr<ShaderPipeline> terrainPipeline;
	s_ptr<ShaderPipeline> pickerPipeline;
	s_ptr<ShaderPipeline> flatColor;

	int pointLightsNum = 0;
	int directionalLightNum = 0;
	int spotLifhtNum = 0;

	int width;
	int height;

	const UINT uavs = 0;

	const UINT offset = 0;
	const float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const float ShadowClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	s_ptr<Hollow::VertexBuffer>		quadVB;
	s_ptr<Hollow::IndexBuffer>		quadIB;

	Hollow::Vector4 AABBplane[6];
	Hollow::Mesh* lightCube;
	Hollow::Material lightMaterial;
	LightInfo lightInfo;
	Hollow::Matrix4 viewProjection;

	s_ptr<DepthStencil> less;
	s_ptr<DepthStencil> greater;

	s_ptr<RasterizerState> cullBack;
	s_ptr<RasterizerState> cullFront;
	s_ptr<RasterizerState> cullNone;

	s_ptr<SamplerState> sampler;
public:
	RenderSystem(RenderApi* renderer, int width, int height) :
		renderer(renderer), width(width), height(height)
	{
		// Gpu buffers
		{
			m_WVPConstantBuffer = GPUBufferManager::instance()->create(0, sizeof(WVP));
			shadowConstantBuffer = GPUBufferManager::instance()->create(1, sizeof(ShadowStruct));

			perModel = GPUBufferManager::instance()->create(2, sizeof(PerModel));
			perMesh = GPUBufferManager::instance()->create(3, sizeof(PerMesh));

			materialConstantBuffer = GPUBufferManager::instance()->create(4, sizeof(MaterialData));
			lightInfoBuffer = GPUBufferManager::instance()->create(5, sizeof(LightInfo));
			boneInfo = GPUBufferManager::instance()->create(6, sizeof(Matrix4));
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


			if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) 
			{
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/gbuffer.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/gbuffer.hlsl"), "main" });

					gBufferPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/depth.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/depth.hlsl"), "main" });

					depthPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/light.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/light.hlsl"), "main" });

					lightPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/SkyMap.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/SkyMap.hlsl"), "main" });

					skyMapPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/terrain.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/terrain.hlsl"), "main" });

					terrainPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/picker.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/picker.hlsl"), "main" });

					pickerPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/flatColor.hlsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/flatColor.hlsl"), "main" });

					flatColor = Hollow::ShaderPipeline::create(pipelineDesc);
				}
			} 
			else 
			{
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/gbuffer.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/gbuffer.glsl"), "main" });

					gBufferPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/depth.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/depth.glsl"), "main" });

					depthPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/light.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/light.glsl"), "main" });

					lightPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/SkyMap.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/SkyMap.glsl"), "main" });

					skyMapPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/terrain.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/terrain.glsl"), "main" });

					terrainPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/picker.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/picker.glsl"), "main" });

					pickerPipeline = Hollow::ShaderPipeline::create(pipelineDesc);
				}
				{
					Hollow::SHADER_PIPELINE_DESC pipelineDesc = { 0 };
					pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/flatColor.glsl"), "main" });
					pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/flatColor.glsl"), "main" });

					flatColor = Hollow::ShaderPipeline::create(pipelineDesc);
				}
			}
		}

		// Render targets
		{
			Hollow::RENDER_TARGET_DESC desc;
			desc.count = 3;
			desc.width = this->width;
			desc.height = this->height;
			desc.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R32G32B32A32;
			gBuffer = RenderTargetManager::instance()->create(desc);
		}
		{
			Hollow::RENDER_TARGET_DESC desc;
			desc.count = 1;
			desc.width = this->width;
			desc.height = this->height;
			desc.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8;
			picker = RenderTargetManager::instance()->create(desc);
		}
		{
			Hollow::RENDER_TARGET_DESC desc;
			desc.count = 1;
			desc.width = 4096;
			desc.height = 4096;
			desc.textureFormat = Hollow::RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8;

			shadow.renderTarget = RenderTargetManager::instance()->create(desc);
			shadow.shadowCamera = new Camera(false);
			shadow.shadowCamera->setOrthoValues(-1000, 1000, -1000, 1000, -1000, 2000);
			shadow.texelSize = Hollow::Vector2(1.0f / desc.width, 1.0f / desc.height);
			shadow.bias = 0.002f;
		}

		{
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
		}

		lightCube = getCube();

		renderer->setViewport(0, 0, this->width, this->height);

		{
			DEPTH_STENCIL_STATE_DESC depthDesc;
			depthDesc.depthFunc = ComparisonFunction::CMP_LESS;
			less = DepthStencil::create(depthDesc);
		}
		{
			DEPTH_STENCIL_STATE_DESC depthDesc;
			depthDesc.depthEnable = true;
			depthDesc.depthFunc = ComparisonFunction::CMP_GREATER;
			greater = DepthStencil::create(depthDesc);
		}
		{
			DEPTH_STENCIL_STATE_DESC depthDesc;
			depthDesc.depthEnable = true;
			depthDesc.depthFunc = ComparisonFunction::CMP_LESS;

			less = DepthStencil::create(depthDesc);
		}

		{
			RASTERIZER_STATE_DESC desc;
			desc.cullMode = CullMode::CLM_BACK;
			cullBack = RasterizerState::create(desc);
		}
		{
			RASTERIZER_STATE_DESC desc;
			desc.cullMode = CullMode::CLM_FRONT;
			cullFront = RasterizerState::create(desc);
		}
		{
			RASTERIZER_STATE_DESC desc;
			desc.cullMode = CullMode::CLM_NONE;
			cullNone = RasterizerState::create(desc);
		}

		SAMPLER_STATE_DESC desc;
		desc.magFilterMode = FilterMode::FM_LINEAR;
		desc.minFilterModel = FilterMode::FM_LINEAR;
		desc.mipFilterMode = FilterMode::FM_LINEAR;
		desc.comparisonFunction = ComparisonFunction::CMP_NEVER;
		sampler = RenderStateManager::instance()->createSamplerState(desc);

		renderer->setSampler(0, sampler);
		renderer->setSampler(1, sampler);

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
		viewProjection = m_Camera->getProjectionMatrix() * m_Camera->getViewMatrix();

		culled = 0;
		renderer->setInputLayout(defaultLayout);
		shadow.shadowCamera->update(dt);
		//calculateAABBPlane();

		if (ProjectSettings::instance()->isProjectLoaded) {
			updateWVP(this->m_Camera);
			// GBuffer pass
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

			if (InputManager::GetKeyboardKeyIsPressed(eKeyCodes::KEY_CONTROL) 
				&& InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_LEFT)
			) {
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

	// Update world view projection matrix
	void updateWVP(Camera* camera)
	{
		m_wvp.WVP = camera->getProjectionMatrix() * camera->getViewMatrix();
		m_wvp.cameraPosition = camera->getPositionVec3();
		m_wvp.cameraViewDirection = camera->getRotationVec3();
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
		renderer->setShaderPipeline(skyMapPipeline);

		renderer->setVertexBuffer(skyMap->mesh->models[0]->vBuffer);
		renderer->setIndexBuffer(skyMap->mesh->models[0]->iBuffer);
		renderer->drawIndexed(skyMap->mesh->models[0]->iBuffer->mHardwareBuffer->getSize());
	}

	void calculateAABBPlane()
	{
		Hollow::Matrix4 projection = m_Camera->getProjectionMatrix() * m_Camera->getViewMatrix();

		AABBplane[0].x = projection.md[3][0] + projection.md[0][0];
		AABBplane[0].y = projection.md[3][1] + projection.md[0][1];
		AABBplane[0].z = projection.md[3][2] + projection.md[0][2];
		AABBplane[0].w = projection.md[3][3] + projection.md[0][3];
		// Right clipping plane
		AABBplane[1].x = projection.md[3][0] - projection.md[0][0];
		AABBplane[1].y = projection.md[3][1] - projection.md[0][1];
		AABBplane[1].z = projection.md[3][2] - projection.md[0][2];
		AABBplane[1].w = projection.md[3][3] - projection.md[0][3];
		// Top clipping plane
		AABBplane[2].x = projection.md[3][0] - projection.md[1][0];
		AABBplane[2].y = projection.md[3][1] - projection.md[1][1];
		AABBplane[2].z = projection.md[3][2] - projection.md[1][2];
		AABBplane[2].w = projection.md[3][3] - projection.md[1][3];
		// Bottom clipping plane
		AABBplane[3].x = projection.md[3][0] + projection.md[1][0];
		AABBplane[3].y = projection.md[3][1] + projection.md[1][1];
		AABBplane[3].z = projection.md[3][2] + projection.md[1][2];
		AABBplane[3].w = projection.md[3][3] + projection.md[1][3];
		// Near clipping plane
		AABBplane[4].x = projection.md[3][0] + projection.md[2][0];
		AABBplane[4].y = projection.md[3][1] + projection.md[2][1];
		AABBplane[4].z = projection.md[3][2] + projection.md[2][2];
		AABBplane[4].w = projection.md[3][3] + projection.md[2][3];
		// Far clipping plane
		AABBplane[5].x = projection.md[3][0] - projection.md[2][0];
		AABBplane[5].y = projection.md[3][1] - projection.md[2][1];
		AABBplane[5].z = projection.md[3][2] - projection.md[2][2];
		AABBplane[5].w = projection.md[3][3] - projection.md[2][3];


		for (int i = 0; i < 6; i++) 
		{
			float length = sqrt((AABBplane[i].x * AABBplane[i].x) + (AABBplane[i].y * AABBplane[i].y) + (AABBplane[i].z * AABBplane[i].z));
			AABBplane[i].x /= length;
			AABBplane[i].y /= length;
			AABBplane[i].z /= length;
			AABBplane[i].w /= length;
		}
	}

	void gBufferPass()
	{
		renderer->setDepthStencilState(less);
		renderer->setRasterizerState(cullBack);
		renderer->setRenderTarget(gBuffer);
		renderer->setShaderPipeline(gBufferPipeline);

		for (auto& entity : EntityManager::instance()->container<GameObject>()) 
		{
			if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>())
			{
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				Hollow::Matrix4 trs = Matrix4::transpose(
					Matrix4::scaling(transform->scale)
					* Matrix4::rotation(transform->rotation)
					* Matrix4::translation(transform->position)
				);

				Hollow::Vector4 A = Hollow::Vector4(renderable->A.x, renderable->A.y, renderable->A.z, 1.0f) 
					* Matrix4::transpose(trs);
				Hollow::Vector4 B = Hollow::Vector4(renderable->B.x, renderable->B.y, renderable->B.z, 1.0f) 
					* Matrix4::transpose(trs);

				perModelData.transform = trs;
				perModel->update(&perModelData);
				renderer->setGpuBuffer(perModel);

				for (auto& object : renderable->renderables) 
				{
					Hollow::Material* material = renderable->materials[object->material];
					if (material->diffuseTexture != nullptr) 
					{
						renderer->setTexture(0, material->diffuseTexture);
					}
					else
					{
						renderer->unsetTexture(0);
					}
					materialConstantBuffer->update(&renderable->materials[object->material]->materialData);
					renderer->setGpuBuffer(materialConstantBuffer);

					renderer->setVertexBuffer(object->vBuffer);
					renderer->setIndexBuffer(object->iBuffer);
					renderer->drawIndexed(object->iBuffer->mHardwareBuffer->getSize());
				}
			}
		}

		for (auto& entity : EntityManager::instance()->container<Light>()) 
		{
			if (entity.hasComponent<LightComponent>()) 
			{
				LightComponent* lightComponent = entity.getComponent<LightComponent>();

				perModelData.transform = Matrix4::transpose(Matrix4::translation(lightComponent->lightData.position));
				perModel->update(&perModelData);
				renderer->setGpuBuffer(perModel);

				renderer->setVertexBuffer(lightCube->models[0]->vBuffer);
				renderer->setIndexBuffer(lightCube->models[0]->iBuffer);
				renderer->drawIndexed(lightCube->models[0]->iBuffer->mHardwareBuffer->getSize());
			}
		}

		renderer->setInputLayout(terrainLayout);
		renderer->setShaderPipeline(terrainPipeline);
		for (auto& entity : EntityManager::instance()->container<Terrain>())
		{
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<TerrainData>()) 
			{
				TerrainData* data = entity.getComponent<TerrainData>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				if (data->vBuffer != nullptr) 
				{
					perModelData.transform = Matrix4::transpose(
						Matrix4::scaling(transform->scale)
						* Matrix4::rotation(transform->rotation)
						* Matrix4::translation(transform->position)
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

		for (auto& entity : EntityManager::instance()->container<GameObject>()) 
		{
			if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) 
			{
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				perModelData.transform = Matrix4::transpose(
					Matrix4::scaling(transform->scale)
					* Matrix4::rotation(transform->rotation)
					* Matrix4::translation(transform->position)
				);
				perModel->update(&perModelData);
				renderer->setGpuBuffer(perModel);

				for (auto& object : renderable->renderables) 
				{
					renderer->setVertexBuffer(object->vBuffer);
					renderer->setIndexBuffer(object->iBuffer);
					renderer->drawIndexed(object->iBuffer->mHardwareBuffer->getSize());
				}
			}
		}
		for (auto& entity : EntityManager::instance()->container<Terrain>()) 
		{
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<TerrainData>()) 
			{
				TerrainData* data = entity.getComponent<TerrainData>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				if (data->vBuffer != nullptr) {
					perModelData.transform = Matrix4::transpose(
						Matrix4::scaling(transform->scale)
						* Matrix4::rotation(transform->rotation)
						* Matrix4::translation(transform->position)
					);

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

		for (auto& entity : EntityManager::instance()->container<GameObject>()) 
		{
			if (entity.hasComponent<RenderableComponent>() && entity.hasComponent<TransformComponent>()) 
			{
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				Hollow::Matrix4 trs = Matrix4::scaling(transform->scale)
					* Matrix4::rotation(transform->rotation)
					* Matrix4::translation(transform->position);

				perModelData.transform = Matrix4::transpose(trs);

				perModelData.color = Vector3(
					((entity.getId() & 0x000000FF) >> 0) / 255.0f, 
					((entity.getId() & 0x0000FF00) >> 8) / 255.0f, 
					((entity.getId() & 0x00FF0000) >> 16) / 255.0f
				);

				perModel->update(&perModelData);
				renderer->setGpuBuffer(perModel);

				for (auto& object : renderable->renderables)
				{
					renderer->setVertexBuffer(object->vBuffer);
					renderer->setIndexBuffer(object->iBuffer);
					renderer->drawIndexed(object->iBuffer->mHardwareBuffer->getSize());
				}
			}
		}
	}

	__forceinline bool aabbCull(const Hollow::Vector3& position, const Hollow::Vector4& min, const Hollow::Vector4& max)
	{
		bool cull = true;
		// Loop through each frustum plane
		for (int i = 0; i < 6; i++) {
			if (AABBplane[i].x * min.x + AABBplane[i].y * min.y + AABBplane[i].z * min.z + AABBplane[i].w > 0) continue;
			if (AABBplane[i].x * min.x + AABBplane[i].y * min.y + AABBplane[i].z * max.z + AABBplane[i].w > 0) continue;
			if (AABBplane[i].x * min.x + AABBplane[i].y * max.y + AABBplane[i].z * min.z + AABBplane[i].w > 0) continue;
			if (AABBplane[i].x * min.x + AABBplane[i].y * max.y + AABBplane[i].z * max.z + AABBplane[i].w > 0) continue;
			if (AABBplane[i].x * max.x + AABBplane[i].y * min.y + AABBplane[i].z * min.z + AABBplane[i].w > 0) continue;
			if (AABBplane[i].x * max.x + AABBplane[i].y * min.y + AABBplane[i].z * max.z + AABBplane[i].w > 0) continue;
			if (AABBplane[i].x * max.x + AABBplane[i].y * max.y + AABBplane[i].z * min.z + AABBplane[i].w > 0) continue;
			if (AABBplane[i].x * max.x + AABBplane[i].y * max.y + AABBplane[i].z * max.z + AABBplane[i].w > 0) continue;

			return true;
			/*cull &= std::max(min.x * AABBplane[i].x, max.x * AABBplane[i].x)
				+ std::max(min.y * AABBplane[i].y, max.y * AABBplane[i].y)
				+ std::max(min.z * AABBplane[i].z, max.z * AABBplane[i].z)
				+ AABBplane[i].w > 0.0f;*/
		}

		return false;
	}

	__forceinline bool OBBInFrustum(const Vector3& Min, const Vector3& Max, Matrix4& obj_transform_mat)
	{
		//трансформируем 8 вершин бокса сразу в clip-space
		//В clip-space пространстве фрустум представляет собой ортонормированный единичный куб [-1..1].
		//Можно очень легко понять, находятся ли все 8 вершин за какой либо плоскостью.
		//Пометка: в DirectX по оси z clip-box имеет размеры 0..1 (вместо -1..1 как в OpenGL), это стоит учесть в коде

		//матрица трансформаций точек в clip-space
		Matrix4 to_clip_space_mat = viewProjection * obj_transform_mat;

		//трансформируем 8 локальных точек бокса в clip-space
		Vector4 obb_points[8];
		obb_points[0] = to_clip_space_mat * Vector4(Min.x, Max.y, Min.z, 1.f);
		obb_points[1] = to_clip_space_mat * Vector4(Min.x, Max.y, Max.z, 1.f);
		obb_points[2] = to_clip_space_mat * Vector4(Max.x, Max.y, Max.z, 1.f);
		obb_points[3] = to_clip_space_mat * Vector4(Max.x, Max.y, Min.z, 1.f);
		obb_points[4] = to_clip_space_mat * Vector4(Max.x, Min.y, Min.z, 1.f);
		obb_points[5] = to_clip_space_mat * Vector4(Max.x, Min.y, Max.z, 1.f);
		obb_points[6] = to_clip_space_mat * Vector4(Min.x, Min.y, Max.z, 1.f);
		obb_points[7] = to_clip_space_mat * Vector4(Min.x, Min.y, Min.z, 1.f);

		bool outside = false, outside_positive_plane, outside_negative_plane;
		//имеем 6 плоскостей отсечения, 3 потому что тестируем 2 плоскости за раз (+1 и -1)
		//находятся ли все 8 точек за плоскостью?
		//в общем то в приведенном коде можно было выполнить нормализацию координаты, делением на w (xyz / w),
		//после чего сравнить с -1 и 1. Если координаты < -1 или > 1, то объект вне фрустума
		outside_positive_plane =
			obb_points[0].x > obb_points[0].w &&
			obb_points[1].x > obb_points[1].w &&
			obb_points[2].x > obb_points[2].w &&
			obb_points[3].x > obb_points[3].w &&
			obb_points[4].x > obb_points[4].w &&
			obb_points[5].x > obb_points[5].w &&
			obb_points[6].x > obb_points[6].w &&
			obb_points[7].x > obb_points[7].w;

		//для DirectX для z координаты (i=3) следует сравнивать с 0
		outside_negative_plane =
			obb_points[0].x < -obb_points[0].w &&
			obb_points[1].x < -obb_points[1].w &&
			obb_points[2].x < -obb_points[2].w &&
			obb_points[3].x < -obb_points[3].w &&
			obb_points[4].x < -obb_points[4].w &&
			obb_points[5].x < -obb_points[5].w &&
			obb_points[6].x < -obb_points[6].w &&
			obb_points[7].x < -obb_points[7].w;

		outside_positive_plane =
			obb_points[0].y > obb_points[0].w &&
			obb_points[1].y > obb_points[1].w &&
			obb_points[2].y > obb_points[2].w &&
			obb_points[3].y > obb_points[3].w &&
			obb_points[4].y > obb_points[4].w &&
			obb_points[5].y > obb_points[5].w &&
			obb_points[6].y > obb_points[6].w &&
			obb_points[7].y > obb_points[7].w;

		//для DirectX для z координаты (i=3) следует сравнивать с 0
		outside_negative_plane =
			obb_points[0].y < -obb_points[0].w &&
			obb_points[1].y < -obb_points[1].w &&
			obb_points[2].y < -obb_points[2].w &&
			obb_points[3].y < -obb_points[3].w &&
			obb_points[4].y < -obb_points[4].w &&
			obb_points[5].y < -obb_points[5].w &&
			obb_points[6].y < -obb_points[6].w &&
			obb_points[7].y < -obb_points[7].w;

		outside_positive_plane =
			obb_points[0].z > obb_points[0].w &&
			obb_points[1].z > obb_points[1].w &&
			obb_points[2].z > obb_points[2].w &&
			obb_points[3].z > obb_points[3].w &&
			obb_points[4].z > obb_points[4].w &&
			obb_points[5].z > obb_points[5].w &&
			obb_points[6].z > obb_points[6].w &&
			obb_points[7].z > obb_points[7].w;

		//для DirectX для z координаты (i=3) следует сравнивать с 0
		outside_negative_plane =
			obb_points[0].z < -obb_points[0].w &&
			obb_points[1].z < -obb_points[1].w &&
			obb_points[2].z < -obb_points[2].w &&
			obb_points[3].z < -obb_points[3].w &&
			obb_points[4].z < -obb_points[4].w &&
			obb_points[5].z < -obb_points[5].w &&
			obb_points[6].z < -obb_points[6].w &&
			obb_points[7].z < -obb_points[7].w;

		outside = outside || outside_positive_plane || outside_negative_plane;

		return outside;
	}

	void updateLight()
	{
		int counter = 0;
		memset(&lightInfo, 0, sizeof(LightInfo));
		for (auto& entity : EntityManager::instance()->container<Light>()) 
		{
			if (entity.hasComponent<LightComponent>()) 
			{
				LightComponent* lightComponent = entity.getComponent<LightComponent>();
				lightInfo.lightData[counter++] = lightComponent->lightData;
			}
		}
		lightInfo.numLights = counter;
		lightInfoBuffer->update(&lightInfo);
		renderer->setGpuBuffer(lightInfoBuffer);
	}
};
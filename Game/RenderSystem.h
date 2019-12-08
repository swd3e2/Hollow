#pragma once

#include <Hollow/Graphics/RenderApi.h>
#include <Hollow/ECS/System.h>
#include "RenderUtils.h"

class RenderSystem : public Hollow::System<RenderSystem>
{
private:
	Hollow::RenderApi* renderApi;
	Hollow::Camera* camera;

	WorldViewProjectionData worldViewProjectionData;

	Hollow::s_ptr<Hollow::GPUBuffer> worldViewProjectionGBuffer;

	Hollow::s_ptr<Hollow::ShaderPipeline> defaultShader;

	Hollow::s_ptr<Hollow::VertexBuffer> simpleQuadVB;
	Hollow::s_ptr<Hollow::IndexBuffer> simpleQuadIB;

	Hollow::s_ptr<Hollow::InputLayout> defaultInputLayout;

	const float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
public:
	RenderSystem(Hollow::RenderApi* renderApi, Hollow::Camera* camera) :
		renderApi(renderApi), camera(camera)
	{
		worldViewProjectionGBuffer = Hollow::GPUBuffer::create(0, sizeof(WorldViewProjectionData));

		{
			Hollow::SHADER_PIPELINE_DESC desc;
			desc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, "shaders/defaultVS.glsl", "main" });
			desc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, "shaders/defaultPS.glsl", "main" });

			defaultShader = Hollow::ShaderPipeline::create(desc);
		}
		{
			Vertex vertices[] = {
				Vertex({  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f }),
				Vertex({  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }),
				Vertex({ -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f }),
				Vertex({ -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f }),
			};
			simpleQuadVB = Hollow::VertexBuffer::create({ vertices, 4, sizeof(Vertex) });

			unsigned int indices[] = { 0, 1, 2, 2, 1, 3 };
			simpleQuadIB = Hollow::IndexBuffer::create({ indices, 6, Hollow::IndexFormat::IFT_UINT });
		}
		{
			defaultInputLayout = Hollow::InputLayout::create({
				{ Hollow::INPUT_DATA_TYPE::Float3, "POSITION" },
				{ Hollow::INPUT_DATA_TYPE::Float2, "TEXCOORD" }
			});
		}

		renderApi->setInputLayout(defaultInputLayout);
		renderApi->setPrimitiveTopology(Hollow::PrimitiveTopology::PT_TRIANGELIST);
		renderApi->setShaderPipeline(defaultShader);
	}

	~RenderSystem()
	{
	}

	virtual void PreUpdate(double dt) override
	{
		renderApi->clearRenderTarget(0, (float*)clearColor);
	}

	virtual void Update(double dt) override
	{
		worldViewProjectionData.ProjectionMatrix = camera->getProjectionMatrix();
		worldViewProjectionData.ViewMatrix = camera->getViewMatrix();
		worldViewProjectionGBuffer->update(&worldViewProjectionData);
		renderApi->setGpuBuffer(worldViewProjectionGBuffer);

		renderApi->setVertexBuffer(simpleQuadVB);
		renderApi->setIndexBuffer(simpleQuadIB);

		renderApi->drawIndexed(6);
	}

	virtual void PostUpdate(double dt) override
	{
		renderApi->present();
	}
private:

};
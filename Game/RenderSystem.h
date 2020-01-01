		#pragma once

#include <Hollow/Graphics/RenderApi.h>
#include <Hollow/ECS/System.h>
#include "RenderUtils.h"
#include "GameObject.h"
#include <Hollow/ECS/EntityManager.h>
#include "RenderComponent.h"
#include "TransformComponent.h"

class RenderSystem : public Hollow::System<RenderSystem>
{
private:
	Hollow::RenderApi* renderApi;
	Hollow::Camera* camera;

	WorldViewProjectionData worldViewProjectionData;
	RenderData renderData;

	Hollow::s_ptr<Hollow::GPUBuffer> worldViewProjectionGBuffer;
	Hollow::s_ptr<Hollow::GPUBuffer> renderGBuffer;

	Hollow::s_ptr<Hollow::ShaderPipeline> defaultShader;

	Hollow::s_ptr<Hollow::InputLayout> defaultInputLayout;

	const float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
public:
	RenderSystem(Hollow::RenderApi* renderApi, Hollow::Camera* camera) :
		renderApi(renderApi), camera(camera)
	{
		worldViewProjectionGBuffer = Hollow::GPUBuffer::create(0, sizeof(WorldViewProjectionData));
		renderGBuffer = Hollow::GPUBuffer::create(1, sizeof(RenderData));

		{
			Hollow::SHADER_PIPELINE_DESC desc;
			desc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, "shaders/defaultVS.glsl", "main" });
			desc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, "shaders/defaultPS.glsl", "main" });

			defaultShader = Hollow::ShaderPipeline::create(desc);
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

		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<RenderComponent>() && entity.hasComponent<TransformComponent>()) {
				RenderComponent* render = entity.getComponent<RenderComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				renderData.transformMatrix = Hollow::Matrix4::rotation(transform->rotation);
				renderApi->setGpuBuffer(renderGBuffer);

				renderApi->setVertexBuffer(render->vertexBuffer);
				renderApi->setIndexBuffer(render->indexBuffer);

				renderApi->drawIndexed(render->indexBuffer->mHardwareBuffer->getSize());
			}
		}
	}

	virtual void PostUpdate(double dt) override
	{
		renderApi->present();
	}
private:

};
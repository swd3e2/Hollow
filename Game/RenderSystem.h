		#pragma once

#include <Hollow/Graphics/RenderApi.h>
#include <Hollow/ECS/System.h>
#include "RenderUtils.h"
#include "GameObject.h"
#include <Hollow/ECS/EntityManager.h>
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "PlayerComponent.h"
#include "Bullet.h"
#include <iostream>

std::ostream& operator<<(std::ostream& cout, const Hollow::Matrix4& mat)
{
	cout << mat.r[0].x << " " << mat.r[0].y << " " << mat.r[0].z << " " << mat.r[0].w << "\n"
		<< mat.r[1].x << " " << mat.r[1].y << " " << mat.r[1].z << " " << mat.r[1].w << "\n"
		<< mat.r[2].x << " " << mat.r[2].y << " " << mat.r[2].z << " " << mat.r[2].w << "\n"
		<< mat.r[3].x << " " << mat.r[3].y << " " << mat.r[3].z << " " << mat.r[3].w << "\n";

	return cout;
}

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
	Hollow::s_ptr<Hollow::ShaderPipeline> cursorShader;

	Hollow::s_ptr<Hollow::InputLayout> defaultInputLayout;

	const float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	Hollow::Vector4 cursorPosition;
	Hollow::Vector4 temp;
	Hollow::s_ptr<Hollow::VertexBuffer> lineVB;
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
			Hollow::SHADER_PIPELINE_DESC desc;
			desc.vertexShader = Hollow::Shader::create({ Hollow::ShaderType::ST_VERTEX, "shaders/cursorVS.glsl", "main" });
			desc.pixelShader = Hollow::Shader::create({ Hollow::ShaderType::ST_PIXEL, "shaders/cursorPS.glsl", "main" });

			cursorShader = Hollow::ShaderPipeline::create(desc);
		}
		{
			defaultInputLayout = Hollow::InputLayout::create({
				{ Hollow::INPUT_DATA_TYPE::Float3, "POSITION" },
				{ Hollow::INPUT_DATA_TYPE::Float2, "TEXCOORD" }
			});
		}
		{
			std::vector<Vertex> vertices;
			vertices.push_back(Vertex({ 0.0f,  10.0f, 0.0f }, { 0.0f, 0.0f }));
			vertices.push_back(Vertex({ 0.0f, -10.0f, 0.0f }, { 0.0f, 0.0f }));

			Hollow::VERTEX_BUFFER_DESC desc;
			desc.data = vertices.data();
			desc.size = vertices.size();
			desc.stride = sizeof(Vertex);
			desc.isDynamic = true;

			lineVB = Hollow::VertexBuffer::create(desc);
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
		cursorPosition.x =  (Hollow::InputManager::mcx / 1920.0f * 2.0f - 1.0f);
		cursorPosition.y = -(Hollow::InputManager::mcy / 1080.0f * 2.0f - 1.0f);
		cursorPosition.z = 0.0f;
		cursorPosition.w = 1.0f;

		renderApi->clearRenderTarget(0, (float*)clearColor);
		renderApi->setInputLayout(defaultInputLayout);
	}

	virtual void Update(double dt) override
	{
		worldViewProjectionData.ProjectionMatrix = camera->getProjectionMatrix();
		worldViewProjectionData.ViewMatrix = camera->getViewMatrix();
		worldViewProjectionGBuffer->update(&worldViewProjectionData);
		renderApi->setGpuBuffer(worldViewProjectionGBuffer);

		Hollow::Matrix4 tempmat = Hollow::Matrix4::transpose(camera->getViewMatrix()) * camera->getProjectionMatrix();
		Hollow::Matrix4 invtempmat = Hollow::Matrix4::inverse(tempmat);

		temp = cursorPosition * invtempmat;
		temp.w = 1.0 / temp.w;

		temp.x *= temp.w;
		temp.y *= temp.w;
		temp.z = 0.0f;

		Hollow::Log::instance()->debug("x {} y {} z {}", temp.x, temp.y, temp.z);

		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<RenderComponent>() && entity.hasComponent<TransformComponent>()) {
				RenderComponent* render = entity.getComponent<RenderComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				transform->screenPos = transform->position * camera->getViewMatrix() * camera->getProjectionMatrix();
				transform->screenPos.y = -transform->screenPos.y;

				if (render->texture != nullptr) {
					renderApi->setTexture(0, render->texture);
				} else {
					renderApi->unsetTexture(0);
				}

				renderData.hasAmbientTexture = render->texture != nullptr;
				renderData.transformMatrix = Hollow::Matrix4::translation(transform->position) * Hollow::Matrix4::rotation(transform->rotation);
				renderGBuffer->update(&renderData);
				renderApi->setGpuBuffer(renderGBuffer);

				renderApi->setVertexBuffer(render->vertexBuffer);
				renderApi->setIndexBuffer(render->indexBuffer);

				renderApi->drawIndexed(render->indexBuffer->mHardwareBuffer->getSize());
			}

			if (entity.hasComponent<PlayerComponent>() && entity.hasComponent<TransformComponent>()) {

				TransformComponent* transform = entity.getComponent<TransformComponent>();

				renderData.hasAmbientTexture = false;
				renderData.transformMatrix = Hollow::Matrix4::identity();
				renderGBuffer->update(&renderData);
				renderApi->setGpuBuffer(renderGBuffer);

				renderApi->setPrimitiveTopology(Hollow::PrimitiveTopology::PT_LINELIST);
				Vertex vertices[2] = {
					Vertex(Hollow::Vector3(transform->position.x, transform->position.y, transform->position.z) , { 0.0f, 0.0f }),
					Vertex(Hollow::Vector3(temp.x, temp.y, temp.z), { 0.0f, 0.0f })
				};
				lineVB->update(vertices, sizeof(Vertex) * 2);
				renderApi->setVertexBuffer(lineVB);
				renderApi->setShaderPipeline(cursorShader);
				//renderApi->draw(2);
				renderApi->setShaderPipeline(defaultShader);
				renderApi->draw(2);

				renderApi->setPrimitiveTopology(Hollow::PrimitiveTopology::PT_TRIANGELIST);
			}
		}

		for (auto& entity : Hollow::EntityManager::instance()->container<Bullet>()) {
			if (entity.hasComponent<RenderComponent>() && entity.hasComponent<TransformComponent>()) {
				RenderComponent* render = entity.getComponent<RenderComponent>();
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				transform->screenPos = transform->position * camera->getViewMatrix() * camera->getProjectionMatrix();
				transform->screenPos.y = -transform->screenPos.y;
				
				renderData.hasAmbientTexture = false;
				renderData.transformMatrix = Hollow::Matrix4::translation(transform->position) * Hollow::Matrix4::rotation(transform->rotation);
				renderGBuffer->update(&renderData);
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

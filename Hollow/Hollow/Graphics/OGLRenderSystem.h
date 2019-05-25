#pragma once

#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/ComponentManager.h"
#include "Hollow/ECS/System.h"
#include "Renderer/OpenGL/OGLRenderApi.h"
#include "ShaderManager.h"
#include "Hollow/Test.h"
#include "Hollow/ECS/RenderableComponent.h"
#include "Hollow/ECS/TransformComponent.h"
#include "Hollow/ECS/GameObject.h"

struct OGLWVP
{
	Matrix4 WVP;
	Vector3 cameraPosition;
};

class OGLRenderSystem : public Hollow::System<OGLRenderSystem>
{
public:
	OGLRenderApi* renderer;

	unsigned int viewMatrixUniformId;
	unsigned int projectionMatrixUniformId;

	Camera* camera;
	unsigned int VAO;
	unsigned int shaderProgram;
	unsigned int UBO;

	VertexBuffer* vBuffer;
	IndexBuffer* iBuffer;
	Mesh* mesh;
	OGLWVP wvp;
	GPUBuffer* buffer;
public:
	OGLRenderSystem(OGLRenderApi* api, Camera* camera) :
		camera(camera), renderer(api)
	{
		wvp.WVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();

		buffer = GPUBufferManager::instance()->create(0, sizeof(OGLWVP));

		glEnable(GL_DEPTH_TEST);
	}	


	virtual void PreUpdate(double dt) override
	{
		renderer->clear();
		renderer->SetShader(ShaderManager::instance()->getShader("default"));

		wvp.WVP = camera->GetProjectionMatrix() * camera->GetViewMatrix();

		buffer->update(&wvp);
		renderer->SetGpuBuffer(buffer);
	}

	virtual void Update(double dt) override
	{
		for (auto& entity : EntityManager::instance()->getContainer<GameObject>()->entityList) {
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<RenderableComponent>()) {
				TransformComponent* transform = entity.getComponent<TransformComponent>();
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();

				for (auto& model : renderable->mesh->models) {
					DrawObject(model);
				}
			}
		}
	}

	virtual void PostUpdate(double dt) override
	{
		renderer->Present();
	}

	void DrawObject(Model* model)
	{
		if (model->material != nullptr) {
			if (model->material->diffuse_texture != nullptr) {
				renderer->SetTexture(0, model->material->diffuse_texture);
			}
			if (model->material->normal_texture != nullptr) {
				renderer->SetTexture(1, model->material->normal_texture);
			}
			if (model->material->specular_texture != nullptr) {
				renderer->SetTexture(2, model->material->specular_texture);
			}
		}

		renderer->SetVertexBuffer(model->vBuffer);
		renderer->SetIndexBuffer(model->iBuffer);
		renderer->DrawIndexed(model->iBuffer->getSize());
	}
};
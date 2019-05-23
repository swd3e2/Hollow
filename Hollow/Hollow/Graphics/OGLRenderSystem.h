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

class OGLRenderSystem : public Hollow::System<OGLRenderSystem>
{
public:
	OGLRenderApi* renderer;

	unsigned int viewMatrixUniformId;
	unsigned int projectionMatrixUniformId;

	Camera* camera;
	GLuint VAO;
	GLuint shaderProgram;

	VertexBuffer* vBuffer;
	IndexBuffer* iBuffer;
	Mesh* mesh;
public:
	OGLRenderSystem(OGLRenderApi* api, Camera* camera) :
		camera(camera), renderer(api)
	{
		int shaderProgramId = static_cast<OGLShaderProgram*>(ShaderManager::instance()->getShader("default"))->shaderId;

		projectionMatrixUniformId = glGetUniformLocation(shaderProgramId, "projectionMatrix");
		glUniformMatrix4fv(projectionMatrixUniformId, 1, false, (float*)& camera->GetProjectionMatrix());

		viewMatrixUniformId = glGetUniformLocation(shaderProgramId, "viewMatrix");
		glUniformMatrix4fv(viewMatrixUniformId, 1, false, (float*)& camera->GetViewMatrix());
	}	


	virtual void PreUpdate(float_t dt) override
	{
		renderer->clear();
		renderer->SetShader(ShaderManager::instance()->getShader("default"));

		glUniformMatrix4fv(viewMatrixUniformId, 1, false, (float*)& camera->GetViewMatrix());
	}

	virtual void Update(float_t dt) override
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

	virtual void PostUpdate(float_t dt) override
	{
		renderer->Present();
	}

	void DrawObject(Model* model)
	{
		renderer->SetVertexBuffer(model->vBuffer);
		renderer->SetIndexBuffer(model->iBuffer);
		renderer->DrawIndexed(model->iBuffer->getSize());
	}
};
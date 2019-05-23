#pragma once

#include "Hollow/ECS/System.h"
#include "Renderer/OpenGL/OGLRenderApi.h"
#include "ShaderManager.h"
#include "Hollow/Test.h"

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
		mesh = getCube();

		int shaderProgramId = static_cast<OGLShaderProgram*>(ShaderManager::instance()->getShader("default"))->shaderId;

		projectionMatrixUniformId = glGetUniformLocation(shaderProgramId, "projectionMatrix");
		glUniformMatrix4fv(projectionMatrixUniformId, 1, false, (float*)& camera->GetProjectionMatrix());

		viewMatrixUniformId = glGetUniformLocation(shaderProgramId, "viewMatrix");
		glUniformMatrix4fv(viewMatrixUniformId, 1, false, (float*)& camera->GetViewMatrix());
		glDisable(GL_CULL_FACE);
	}	


	virtual void PreUpdate(float_t dt) override
	{
		renderer->clear();
		renderer->SetShader(ShaderManager::instance()->getShader("default"));

		glUniformMatrix4fv(viewMatrixUniformId, 1, false, (float*)& camera->GetViewMatrix());
	}

	virtual void Update(float_t dt) override
	{
		renderer->SetVertexBuffer(mesh->models[0]->vBuffer);
		renderer->SetIndexBuffer(mesh->models[0]->iBuffer);
		renderer->DrawIndexed(mesh->models[0]->iBuffer->getSize());
	}

	virtual void PostUpdate(float_t dt) override
	{
		renderer->Present();
	}
};
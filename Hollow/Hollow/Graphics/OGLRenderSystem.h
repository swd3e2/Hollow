#pragma once

#include "Hollow/ECS/System.h"
#include "Renderer/OpenGL/OGLRenderApi.h"
#include "ShaderManager.h"

class OGLRenderSystem : public Hollow::System<OGLRenderSystem>
{
public:
	OGLRenderApi* renderer;

	int viewMatrixUniformId;
	int projectionMatrixUniformId;

	Camera* camera;

	VertexBuffer* vBuffer;
	IndexBuffer* iBuffer;
public:
	OGLRenderSystem()
	{
		Vertex cube_vertices[] = {
		Vertex(1.0,  0.0f,  1.0,	1.0, 0.0,	1.0, -1.0, -1.0),
		Vertex(-1.0,  0.0f,  1.0,	0.0, 0.0,   1.0, -1.0, -1.0),
		Vertex(-1.0, -0.0f, -1.0,	0.0, 1.0,	1.0, -1.0, -1.0),
		Vertex(1.0, -0.0f, -1.0,	1.0, 1.0,	1.0, -1.0, -1.0),
		};

		unsigned int indices[] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		vBuffer = HardwareBufferManager::instance()->createVertexBuffer(cube_vertices, 4);
		iBuffer = HardwareBufferManager::instance()->createIndexBuffer(indices, 4);

		int shaderProgramId = static_cast<OGLShaderProgram*>(ShaderManager::instance()->getShader("default"))->shaderId;

		projectionMatrixUniformId = glGetUniformLocation(shaderProgramId, "projectionMatrix");
		glUniformMatrix4fv(projectionMatrixUniformId, 1, false, (float*)& camera->GetProjectionMatrix());

		viewMatrixUniformId = glGetUniformLocation(shaderProgramId, "viewMatrix");
		glUniformMatrix4fv(viewMatrixUniformId, 1, false, (float*)& camera->GetViewMatrix());
	}


	virtual void PreUpdate(float_t dt) override
	{
		renderer->clear();
		glUniformMatrix4fv(viewMatrixUniformId, 1, false, (float*)& camera->GetViewMatrix());
	}

	virtual void Update(float_t dt) override
	{
		renderer->SetVertexBuffer(vBuffer);
		renderer->SetIndexBuffer(iBuffer);
		renderer->DrawIndexed(iBuffer->getSize());
	}

	virtual void PostUpdate(float_t dt) override
	{
		renderer->Present();
	}
};
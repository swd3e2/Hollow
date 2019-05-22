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

		Vertex cube_vertices[] = {
			Vertex(-0.5f, -0.5f, 0.2f,	1.0, 0.0,	1.0, -1.0, -1.0),
			Vertex(0.5f, -0.5f, 0.2f,	0.0, 0.0,   1.0, -1.0, -1.0),
			Vertex(0.5f,  0.5f, 0.2f,	0.0, 1.0,	1.0, -1.0, -1.0),
			Vertex(-0.5f,  0.5f, 0.2f,	1.0, 1.0,	1.0, -1.0, -1.0),
		};

		unsigned int indices[] = {
			0, 1, 2,   // first triangle
		};

		vBuffer = HardwareBufferManager::instance()->createVertexBuffer(cube_vertices, 4);
		iBuffer = HardwareBufferManager::instance()->createIndexBuffer(indices, 4);

		int  success;
		char infoLog[512];

		const char* vertexShaderSource = "#version 330\n"
			"layout(location = 0) in vec3 aPos;"
			"uniform mat4 viewMatrix;"
			"uniform mat4 projectionMatrix;"
			"void main()"
			"{"
			"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);"
			"gl_Position = gl_Position * viewMatrix * projectionMatrix;"
			"}";

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			HW_ERROR("{}", infoLog);
		}

		const char* fragmentShaderSource =
			"#version 330\n"
			"out vec4 FragColor;"
			"void main()"
			"{"
			"FragColor = vec4(1.0f, 0.0f, 0.2f, 0.0f);"
			"}";

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			HW_ERROR("{}", infoLog);
		}

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glUseProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		float vertices[] = {
			-0.5f, -0.5f, 0.2f,
			 0.5f, -0.5f, 0.2f,
			 0.0f,  0.5f, 0.2f
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		unsigned int VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		int shaderProgramId = static_cast<OGLShaderProgram*>(ShaderManager::instance()->getShader("default"))->shaderId;

		projectionMatrixUniformId = glGetUniformLocation(shaderProgram, "projectionMatrix");
		glUniformMatrix4fv(projectionMatrixUniformId, 1, false, (float*)& camera->GetProjectionMatrix());

		viewMatrixUniformId = glGetUniformLocation(shaderProgram, "viewMatrix");
		glUniformMatrix4fv(viewMatrixUniformId, 1, false, (float*)& camera->GetViewMatrix());

		//glCullFace(GL_NONE);
	}	


	virtual void PreUpdate(float_t dt) override
	{
		renderer->clear();
		glUniformMatrix4fv(viewMatrixUniformId, 1, false, (float*)& camera->GetViewMatrix());
	}

	virtual void Update(float_t dt) override
	{
		//renderer->SetShader(ShaderManager::instance()->getShader("default"));
		//renderer->SetIndexBuffer(mesh->models[0]->iBuffer);
		renderer->SetVertexBuffer(vBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(shaderProgram);
		//renderer->DrawIndexed(mesh->models[0]->iBuffer->getSize());
		/*glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); */
	}

	virtual void PostUpdate(float_t dt) override
	{
		renderer->Present();
	}
};
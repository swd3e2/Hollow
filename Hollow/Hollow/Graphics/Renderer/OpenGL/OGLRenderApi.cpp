#include "OGLRenderApi.h"
#include "OGLWindowManager.h"

OGLRenderApi::OGLRenderApi(int width, int height)
{
	windowManager = new OGLWindowManager();
}

void OGLRenderApi::startUp()
{
	int  success;
	char infoLog[512];

	hwnd = static_cast<OGLWin32Window*>(windowManager->getWindow())->getHWND();

	float vertices[] = {
		-0.5f, -0.5f, 0.2f,
		 0.5f, -0.5f, 0.2f,
		 0.0f,  0.5f, 0.2f
	};

	float texCoords[] = {
		0.0f, 0.0f,  // lower-left corner  
		1.0f, 0.0f,  // lower-right corner
		0.5f, 1.0f   // top-center corner
	};
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

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

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBOs[2];
	glGenBuffers(2, VBOs);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 2 * sizeof(float), (void*)0);

	projectionMatrixUniformId = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformId, 1, false, (float*)& camera->GetProjectionMatrix());

	viewMatrixUniformId = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixUniformId, 1, false, (float*)& camera->GetViewMatrix());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void OGLRenderApi::clear()
{
	viewMatrixUniformId = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixUniformId, 1, false, (float*)& camera->GetViewMatrix());

	glViewport(0, 0, 2560, 1440);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	SwapBuffers(GetDC(*hwnd));
}

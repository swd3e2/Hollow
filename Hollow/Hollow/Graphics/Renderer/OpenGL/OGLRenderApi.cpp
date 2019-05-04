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
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	const char* vertexShaderSource = "#version 330\n"
		"layout(location = 0) in vec3 aPos;"
		"void main()"
		"{"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);"
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

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void OGLRenderApi::clear()
{
	glViewport(0, 0, 2560, 1440);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	SwapBuffers(GetDC(*hwnd));
}

#include "OGLShaderManager.h"

OGLShaderManager::OGLShaderManager()
{
	shaderTypeFolder = "OGL";
}

Shader* OGLShaderManager::compileShader(ShaderType type, const std::string& path)
{
	OGLShader* shader = nullptr;

	int  success;
	char infoLog[512];

	switch (type)
	{
	case VERTEX:
	{
		shader = new OGLShader(ShaderType::VERTEX);

		shader->shaderId = glCreateShader(GL_VERTEX_SHADER);
		std::string shaderContent = Hollow::FileSystem::getFileContent(path);
		glShaderSource(shader->shaderId, 1, (const GLchar * const*)shaderContent.c_str(), NULL);
		glCompileShader(shader->shaderId);

		glGetShaderiv(shader->shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader->shaderId, 512, NULL, infoLog);
			HW_ERROR("{}", infoLog);
		}
	} break;
	case PIXEL:
	{
		shader = new OGLShader(ShaderType::PIXEL);

		shader->shaderId = glCreateShader(GL_FRAGMENT_SHADER);
		std::string shaderContent = Hollow::FileSystem::getFileContent(path);
		glShaderSource(shader->shaderId, 1, (const GLchar * const*)shaderContent.c_str(), NULL);
		glCompileShader(shader->shaderId);

		glGetShaderiv(shader->shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader->shaderId, 512, NULL, infoLog);
			HW_ERROR("{}", infoLog);
		}
	} break;
	case GEOMERTY:
	{

	} break;
	case COMPUTE:
	{

	} break;
	case HULL:
	{

	} break;
	case DOMAINS:
	{

	} break;
	}

	return shader;
}

ShaderProgram* OGLShaderManager::createShader(Shader* vertexShader, Shader* pixelShader)
{
	OGLShaderProgram* shaderProgram = new OGLShaderProgram();

	shaderProgram->shaderId = glCreateProgram();
	glAttachShader(shaderProgram->shaderId, static_cast<OGLShader*>(vertexShader)->shaderId);
	glAttachShader(shaderProgram->shaderId, static_cast<OGLShader*>(pixelShader)->shaderId);
	glLinkProgram(shaderProgram->shaderId);

	glDeleteShader(static_cast<OGLShader*>(vertexShader)->shaderId);
	glDeleteShader(static_cast<OGLShader*>(pixelShader)->shaderId);

	return shaderProgram;
}

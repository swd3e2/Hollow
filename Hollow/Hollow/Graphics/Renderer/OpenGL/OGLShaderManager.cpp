#include "OGLShaderManager.h"

OGLShaderManager::OGLShaderManager()
{
	shaderTypeFolder = "OGL";

	std::vector<std::string> shaders = fs.read_directory(shaderFolder + shaderTypeFolder + "/vertex/");

	for (auto& it : shaders)
	{
		if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
		{
			ShaderProgram* shader = createShader(
				compileShader(ShaderType::VERTEX, shaderFolder + shaderTypeFolder + "/vertex/" + it),
				compileShader(ShaderType::PIXEL, shaderFolder + shaderTypeFolder + "/pixel/" + it)
			);
			this->shaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = shader;
		}
	}
}

Shader* OGLShaderManager::compileShader(ShaderType type, const std::string& path)
{
	OGLShader* shader = nullptr;

	int  success;
	char infoLog[512];

	std::string fileContent = Hollow::FileSystem::getFileContent(path);
	const char* shaderContent = fileContent.c_str();

	switch (type)
	{
	case VERTEX:
	{ 
		shader = new OGLShader(ShaderType::VERTEX);

		shader->shaderId = glCreateShader(GL_VERTEX_SHADER);
		
		glShaderSource(shader->shaderId, 1, &shaderContent, NULL);
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
		glShaderSource(shader->shaderId, 1, &shaderContent, NULL);
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

	glUseProgram(shaderProgram->shaderId);

	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram->shaderId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram->shaderId, 512, NULL, infoLog);
		HW_ERROR("{}", infoLog);
	}

	int tex0 = glGetUniformLocation(shaderProgram->shaderId, "ambient_map");
	int tex1 = glGetUniformLocation(shaderProgram->shaderId, "normal_map");
	int tex2 = glGetUniformLocation(shaderProgram->shaderId, "specular_map");
	int tex3 = glGetUniformLocation(shaderProgram->shaderId, "shadow_map");

	glUniform1i(tex0, 0);
	glUniform1i(tex1, 1);
	glUniform1i(tex2, 2);
	glUniform1i(tex3, 3);

	glDeleteShader(static_cast<OGLShader*>(vertexShader)->shaderId);
	glDeleteShader(static_cast<OGLShader*>(pixelShader)->shaderId);

	return shaderProgram;
}

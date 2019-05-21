#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	setStartedUp();

	std::vector<std::string>* shaders;

	shaders = fs.read_directory(shaderFolder + shaderTypeFolder + "/vertex/");

	for (auto& it : *shaders)
	{
		if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
		{
			ShaderProgram* shader = createShader(
				compileShader(ShaderType::PIXEL, shaderFolder + shaderTypeFolder + "/pixel/" + it),
				compileShader(ShaderType::VERTEX, shaderFolder + shaderTypeFolder + "/vertex/" + it)
			);
			this->shaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = shader;
		}
	}
}

ShaderManager::~ShaderManager()
{
	setShutdown();
}

#include "OGLShaderManager.h"

namespace Hollow {
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

	Shader* OGLShaderManager::compileShader(ShaderType type, const std::string& path, Shader* prevShader)
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
			if (prevShader != nullptr) {
				shader = static_cast<OGLShader*>(prevShader);
			}
			else {
				shader = new OGLShader(ShaderType::VERTEX);
			}

			int shaderId = glCreateShader(GL_VERTEX_SHADER);

			glShaderSource(shaderId, 1, &shaderContent, NULL);
			glCompileShader(shaderId);

			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
				glDeleteShader(shaderId);
				HW_ERROR("{}", infoLog);
			}
			else {
				shader->prevShaderId = shader->shaderId;
				shader->shaderId = shaderId;
			}
		} break;
		case PIXEL:
		{
			if (prevShader != nullptr) {
				shader = static_cast<OGLShader*>(prevShader);
			}
			else {
				shader = new OGLShader(ShaderType::PIXEL);
			}

			int shaderId = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(shaderId, 1, &shaderContent, NULL);
			glCompileShader(shaderId);

			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
				glDeleteShader(shaderId);
				HW_ERROR("{}", infoLog);
			}
			else {
				shader->prevShaderId = shader->shaderId;
				shader->shaderId = shaderId;
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

	ShaderProgram* OGLShaderManager::createShader(Shader* vertexShader, Shader* pixelShader, ShaderProgram* prevProgram)
	{
		int  success;
		char infoLog[512];

		OGLShaderProgram* shaderProgram = nullptr;

		if (prevProgram != nullptr) {
			shaderProgram = static_cast<OGLShaderProgram*>(prevProgram);
		}
		else {
			shaderProgram = new OGLShaderProgram(vertexShader, pixelShader);
			shaderProgram->shaderId = glCreateProgram();
		}

		if (static_cast<OGLShader*>(vertexShader)->shaderId == DEFAULT_SHADER_ID || static_cast<OGLShader*>(pixelShader)->shaderId == DEFAULT_SHADER_ID) {
			return shaderProgram;
		}

		if (prevProgram != nullptr) {
			OGLShader* pPixelShader = static_cast<OGLShader*>(prevProgram->getPixelShader());
			if (pPixelShader != nullptr && pPixelShader->prevShaderId != DEFAULT_SHADER_ID) {
				glDetachShader(shaderProgram->shaderId, pPixelShader->prevShaderId);
			}
			OGLShader* pVrtexShader = static_cast<OGLShader*>(prevProgram->getVertexShader());
			if (pVrtexShader != nullptr && pVrtexShader->prevShaderId != DEFAULT_SHADER_ID) {
				glDetachShader(shaderProgram->shaderId, pVrtexShader->prevShaderId);
			}
		}

		glAttachShader(shaderProgram->shaderId, static_cast<OGLShader*>(vertexShader)->shaderId);
		glAttachShader(shaderProgram->shaderId, static_cast<OGLShader*>(pixelShader)->shaderId);
		glLinkProgram(shaderProgram->shaderId);

		glGetProgramiv(shaderProgram->shaderId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram->shaderId, 512, NULL, infoLog);
			HW_ERROR("{}", infoLog);

			shaderProgram->linked = false;
			int size = 0;
			unsigned int attachedShaders[10];
			glGetAttachedShaders(shaderProgram->shaderId, 10, &size, attachedShaders);
			glDeleteShader(static_cast<OGLShader*>(vertexShader)->shaderId);
			glDeleteShader(static_cast<OGLShader*>(pixelShader)->shaderId);

			return shaderProgram;
		}

		glUseProgram(shaderProgram->shaderId);

		int tex0 = glGetUniformLocation(shaderProgram->shaderId, "ambient_map");
		int tex1 = glGetUniformLocation(shaderProgram->shaderId, "normal_map");
		int tex2 = glGetUniformLocation(shaderProgram->shaderId, "specular_map");
		int tex3 = glGetUniformLocation(shaderProgram->shaderId, "shadow_map");
		int tex4 = glGetUniformLocation(shaderProgram->shaderId, "enivironmentMap");

		glUniform1i(tex0, 0);
		glUniform1i(tex1, 1);
		glUniform1i(tex2, 2);
		glUniform1i(tex3, 3);
		glUniform1i(tex4, 4);

		glDeleteShader(static_cast<OGLShader*>(vertexShader)->shaderId);
		glDeleteShader(static_cast<OGLShader*>(pixelShader)->shaderId);

		shaderProgram->linked = true;
		shaderProgram->setPixelShader(pixelShader);
		shaderProgram->setVertexShader(vertexShader);

		return shaderProgram;
	}

	void OGLShaderManager::reloadShader(ShaderProgram* program)
	{
		std::string shaderName;

		for (auto& it : shaders) {
			if (it.second == program) {
				shaderName = it.first;
			}
		}
		if (!shaderName.size()) return;

		ShaderProgram* temp = createShader(
			compileShader(ShaderType::VERTEX, shaderFolder + shaderTypeFolder + "/vertex/" + shaderName + ".glsl", program->getVertexShader()),
			compileShader(ShaderType::PIXEL, shaderFolder + shaderTypeFolder + "/pixel/" + shaderName + ".glsl", program->getPixelShader()),
			program
		);
	}
}
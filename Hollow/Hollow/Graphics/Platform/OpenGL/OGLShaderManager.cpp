#include "OGLShaderManager.h"

namespace Hollow {
	s_ptr<Shader> OGLShaderManager::create(const SHADER_DESC& desc)
	{
		OGLShader* shader = new OGLShader();
		compileInternal(shader->shaderId, desc);

		return s_ptr<Shader>(shader);
	}

	void OGLShaderManager::reload(const s_ptr<Shader>& shader, std::string shaderContent)
	{

	}

	s_ptr<ShaderPipeline> OGLShaderManager::create(const SHADER_PIPELINE_DESC& desc)
	{
		OGLShaderPipeline* pipeline = new OGLShaderPipeline();
		glGenProgramPipelines(1, &pipeline->pipelineId);

		pipeline->setVertexShader(desc.vertexShader);
		pipeline->setPixelShader(desc.pixelShader);
		pipeline->setGeometryShader(desc.geometryShader);
		pipeline->setHullShader(desc.hullShader);
		pipeline->setDomainShader(desc.domainShader);

		return s_ptr<ShaderPipeline>(pipeline);
	}

	bool OGLShaderManager::getError(GLuint shaderId)
	{
		GLint isLinked = 0;
		glGetProgramiv(shaderId, GL_LINK_STATUS, &isLinked);

		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
			GLchar* infoLog = new GLchar[maxLength];
			glGetProgramInfoLog(shaderId, maxLength, &maxLength, infoLog);
			HW_ERROR("{}", infoLog);
			delete[] infoLog;
			return true;
		}
		return false;
	}
	void OGLShaderManager::compileInternal(GLuint& shaderId, const SHADER_DESC& desc)
	{
		std::string shaderCode;
		if (desc.content.size()) {
			shaderCode = desc.content;
		} else {
			shaderCode = FileSystem::getFileContent(desc.filename);
		}
		const char* charShaderCode = shaderCode.c_str();

		switch (desc.type)
		{
		case ShaderType::ST_VERTEX: {
			shaderId = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &charShaderCode);
			getError(shaderId);
		} break;
		case ShaderType::ST_PIXEL: {
			const char* temp;
			int size = 0;
			std::unordered_map<std::string, int> textures;

			const char* entrance = strstr(charShaderCode, "uniform sampler2D");
			while (entrance != nullptr)
			{
				entrance += strlen("uniform sampler2D") + 1; // skip "uniform sampler2D" and space
				temp = strstr(entrance, ";");
				size = temp - entrance;
				char* uniformName = new char[size + 1];
				strncpy(uniformName, entrance, size);
				uniformName[size] = '\0';

				entrance = strstr(entrance, "//");
				entrance += strlen("//");
				temp = strstr(entrance, "\n");
				size = temp - entrance;
				char* slot = new char[size + 1];
				strncpy(slot, entrance, size);
				slot[size] = '\0';

				textures[uniformName] = atoi(slot);
				entrance = strstr(entrance, "uniform sampler2D");
			}

			entrance = strstr(charShaderCode, "uniform samplerCube");
			while (entrance != nullptr)
			{
				entrance += strlen("uniform samplerCube") + 1; // skip "uniform samplerCube" and space
				temp = strstr(entrance, ";");
				size = temp - entrance;
				char* uniformName = new char[size + 1];
				strncpy(uniformName, entrance, size);
				uniformName[size] = '\0';

				entrance = strstr(entrance, "//");
				entrance += strlen("//");
				temp = strstr(entrance, "\n");
				size = temp - entrance;
				char* slot = new char[size + 1];
				strncpy(slot, entrance, size);
				slot[size] = '\0';

				textures[uniformName] = atoi(slot);
				delete[] uniformName;
				delete[] slot;
				entrance = strstr(entrance, "uniform samplerCube");
			}

			shaderId = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &charShaderCode);

			if (!getError(shaderId)) {
				for (auto& it : textures) {
					glProgramUniform1i(shaderId, glGetUniformLocation(shaderId, it.first.c_str()), it.second);
				}
			}
		} break;
		}
	}
}

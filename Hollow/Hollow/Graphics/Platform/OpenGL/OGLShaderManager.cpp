#include "OGLShaderManager.h"

namespace Hollow {
	s_ptr<Shader> OGLShaderManager::create(const SHADER_DESC& desc)
	{
		OGLShader* shader = new OGLShader();
		const char* shaderCode = desc.content.c_str();

		switch (desc.type)
		{
		case ShaderType::ST_VERTEX: {
			shader->shaderId = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &shaderCode);
			getError(shader->shaderId);
		} break;
		case ShaderType::ST_PIXEL: {
			const char* temp;
			int size = 0;
			std::unordered_map<std::string, int> textures;

			const char* entrance = strstr(shaderCode, "uniform sampler2D");
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

			entrance = strstr(shaderCode, "uniform samplerCube");
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

			shader->shaderId = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &shaderCode);

			if (!getError(shader->shaderId)) {
				for (auto& it : textures) {
					glProgramUniform1i(shader->shaderId, glGetUniformLocation(shader->shaderId, it.first.c_str()), it.second);
				}
			}
		} break;
		}
		return s_ptr<Shader>(shader);
	}

	s_ptr<ShaderPipeline> Hollow::OGLShaderManager::create(const SHADER_PIPELINE_DESC& desc)
	{
		OGLShaderPipeline* pipeline = new OGLShaderPipeline();
		glGenProgramPipelines(1, &pipeline->pipelineId);

		if (desc.vertexShader != nullptr)
		{
			pipeline->vertexShader = desc.vertexShader;
			glUseProgramStages(pipeline->pipelineId, GL_VERTEX_SHADER_BIT, std::static_pointer_cast<OGLShader>(desc.vertexShader)->shaderId);
			getError(pipeline->pipelineId);
		}

		if (desc.pixelShader != nullptr)
		{
			pipeline->pixelShader = desc.pixelShader;
			int pixelShader = std::static_pointer_cast<OGLShader>(desc.pixelShader)->shaderId;
			glUseProgramStages(pipeline->pipelineId, GL_FRAGMENT_SHADER_BIT, pixelShader);

			/*glActiveShaderProgram(pipeline->pipelineId, pixelShader);

			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex0"), 0);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex1"), 1);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex2"), 2);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex3"), 3);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex4"), 4);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex5"), 5);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex6"), 6);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex7"), 7);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex8"), 8);

			glActiveShaderProgram(pipeline->pipelineId, 0);*/
		}

		if (desc.geometryShader != nullptr)
		{
			pipeline->geometryShader = desc.geometryShader;
			glUseProgramStages(pipeline->pipelineId, GL_GEOMETRY_SHADER_BIT, std::static_pointer_cast<OGLShader>(desc.geometryShader)->shaderId);
		}

		if (desc.hullShader != nullptr)
		{
			pipeline->hullShader = desc.hullShader;
			glUseProgramStages(pipeline->pipelineId, GL_TESS_CONTROL_SHADER_BIT, std::static_pointer_cast<OGLShader>(desc.hullShader)->shaderId);
		}

		if (desc.domainShader != nullptr)
		{
			pipeline->domainShader = desc.domainShader;
			glUseProgramStages(pipeline->pipelineId, GL_TESS_EVALUATION_SHADER_BIT, std::static_pointer_cast<OGLShader>(desc.domainShader)->shaderId);
		}

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
}

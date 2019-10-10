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
			shader->shaderId = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &shaderCode);
			getError(shader->shaderId);
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
			glUseProgramStages(pipeline->pipelineId, GL_VERTEX_SHADER_BIT, static_cast<OGLShader*>(desc.vertexShader)->shaderId);
		}

		if (desc.pixelShader != nullptr)
		{
			int pixelShader = std::static_pointer_cast<OGLShader>(desc.pixelShader)->shaderId;
			glUseProgramStages(pipeline->pipelineId, GL_FRAGMENT_SHADER_BIT, pixelShader);

			glActiveShaderProgram(pipeline->pipelineId, pixelShader);

			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex0"), 0);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex1"), 1);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex2"), 2);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex3"), 3);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex4"), 4);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex5"), 5);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex6"), 6);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex7"), 7);
			glProgramUniform1i(pixelShader, glGetUniformLocation(pixelShader, "tex8"), 8);

			glActiveShaderProgram(pipeline->pipelineId, 0);
		}

		if (desc.geometryShader != nullptr)
		{
			glUseProgramStages(pipeline->pipelineId, GL_GEOMETRY_SHADER_BIT, dynamic_cast<OGLShader*>(desc.geometryShader)->shaderId);
		}

		if (desc.hullShader != nullptr)
		{
			glUseProgramStages(pipeline->pipelineId, GL_TESS_CONTROL_SHADER_BIT, dynamic_cast<OGLShader*>(desc.hullShader)->shaderId);
		}

		if (desc.domainShader != nullptr)
		{
			glUseProgramStages(pipeline->pipelineId, GL_TESS_EVALUATION_SHADER_BIT, dynamic_cast<OGLShader*>(desc.domainShader)->shaderId);
		}

		return s_ptr<ShaderPipeline>(pipeline);
	}
}

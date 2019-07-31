#include "OGLPipelineStateManager.h"

namespace Hollow {
	PipelineState* OGLPipelineStateManager::create(const PIPELINE_STATE_DESC& desc)
	{
		OGLPipelineState* pipeline = new OGLPipelineState();
		glGenProgramPipelines(1, &pipeline->pipelineId);

		if (desc.vertexShader != nullptr) {
			glUseProgramStages(pipeline->pipelineId, GL_VERTEX_SHADER_BIT, static_cast<OGLShader*>(desc.vertexShader)->shaderId);
		}
		if (desc.pixelShader != nullptr) {
			int pixelShader = static_cast<OGLShader*>(desc.pixelShader)->shaderId;
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
		if (desc.geometryShader != nullptr) {
			glUseProgramStages(pipeline->pipelineId, GL_GEOMETRY_SHADER_BIT, static_cast<OGLShader*>(desc.geometryShader)->shaderId);
		}
		if (desc.hullShader != nullptr) {
			glUseProgramStages(pipeline->pipelineId, GL_TESS_CONTROL_SHADER_BIT, static_cast<OGLShader*>(desc.hullShader)->shaderId);
		}
		if (desc.domainShader != nullptr) {
			glUseProgramStages(pipeline->pipelineId, GL_TESS_EVALUATION_SHADER_BIT, static_cast<OGLShader*>(desc.domainShader)->shaderId);
		}
		if (desc.computeShader != nullptr) {
			glUseProgramStages(pipeline->pipelineId, GL_COMPUTE_SHADER_BIT, static_cast<OGLShader*>(desc.computeShader)->shaderId);
		}

		return pipeline;
	}
}

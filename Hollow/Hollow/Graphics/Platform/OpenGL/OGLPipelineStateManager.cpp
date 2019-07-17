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
			GLint tex0 = glGetUniformLocation(pixelShader, "ambient_map");
			GLint tex1 = glGetUniformLocation(pixelShader, "normal_map");
			GLint tex2 = glGetUniformLocation(pixelShader, "specular_map");
			GLint tex3 = glGetUniformLocation(pixelShader, "shadow_map");
			GLint tex4 = glGetUniformLocation(pixelShader, "environmentMap");

			glProgramUniform1i(pixelShader, tex0, 0);
			glProgramUniform1i(pixelShader, tex1, 1);
			glProgramUniform1i(pixelShader, tex2, 2);
			glProgramUniform1i(pixelShader, tex3, 3);
			glProgramUniform1i(pixelShader, tex4, 4);
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


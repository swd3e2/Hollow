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
			glUseProgramStages(pipeline->pipelineId, GL_FRAGMENT_SHADER_BIT, static_cast<OGLShader*>(desc.pixelShader)->shaderId);

			int tex0 = glGetUniformLocation(static_cast<OGLShader*>(desc.pixelShader)->shaderId, "ambient_map");
			int tex1 = glGetUniformLocation(static_cast<OGLShader*>(desc.pixelShader)->shaderId, "normal_map");
			int tex2 = glGetUniformLocation(static_cast<OGLShader*>(desc.pixelShader)->shaderId, "specular_map");
			int tex3 = glGetUniformLocation(static_cast<OGLShader*>(desc.pixelShader)->shaderId, "shadow_map");
			int tex4 = glGetUniformLocation(static_cast<OGLShader*>(desc.pixelShader)->shaderId, "environmentMap");

			glUniform1i(tex0, 0);
			glUniform1i(tex1, 1);
			glUniform1i(tex2, 2);
			glUniform1i(tex3, 3);
			glUniform1i(tex4, 4);
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

		return nullptr;
	}
}


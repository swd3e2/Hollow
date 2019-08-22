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

	SamplerState* OGLPipelineStateManager::create(const SAMPLER_STATE_DESC& desc)
	{
		OGLSamplerState* sampler = new OGLSamplerState();

		glGenSamplers(1, &sampler->samplerObjectId);

		glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_WRAP_S, OGLHelper::getAddressingMode(desc.addressingMode));
		glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_WRAP_T, OGLHelper::getAddressingMode(desc.addressingMode));
		glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_WRAP_R, OGLHelper::getAddressingMode(desc.addressingMode));

		glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_MAG_FILTER, OGLHelper::getFilteringMode(desc.magFilterMode));
		glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_MIN_FILTER, OGLHelper::getMinMipFilteringMode(desc.minFilterModel, desc.mipFilterMode));

		glSamplerParameterf(sampler->samplerObjectId, GL_TEXTURE_MIN_LOD, desc.minLOD);
		glSamplerParameterf(sampler->samplerObjectId, GL_TEXTURE_MAX_LOD, desc.maxLOD);
		glSamplerParameterf(sampler->samplerObjectId, GL_TEXTURE_LOD_BIAS, desc.mipLODBias);

		if (desc.comparisonFunction != ComparisonFunction::NEVER) {
			glTexParameteri(sampler->samplerObjectId, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glTexParameteri(sampler->samplerObjectId, GL_TEXTURE_COMPARE_FUNC, OGLHelper::getComparisonFunction(desc.comparisonFunction));
		} else {
			glTexParameteri(sampler->samplerObjectId, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		}

		return nullptr;
	}
}


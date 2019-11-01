#pragma once

#include "Hollow/Graphics/ShaderPipeline.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLShaderPipeline : public ShaderPipeline
	{
	private:
		friend class OGLShaderManager;
	public:
		GLuint pipelineId;
	public:
		virtual void setVertexShader(const s_ptr<Shader>& shader) override
		{
			if (shader != nullptr) {
				this->vertexShader = shader;
				glUseProgramStages(this->pipelineId, GL_VERTEX_SHADER_BIT, std::static_pointer_cast<OGLShader>(shader)->shaderId);
				this->vertexShader->parent = this;
			}
		}

		virtual void setPixelShader(const s_ptr<Shader>& shader) override
		{
			if (shader != nullptr) {
				this->pixelShader = shader;
				glUseProgramStages(this->pipelineId, GL_FRAGMENT_SHADER_BIT, std::static_pointer_cast<OGLShader>(shader)->shaderId);
				this->pixelShader->parent = this;
			}
		}

		virtual void setGeometryShader(const s_ptr<Shader>& shader) override
		{
			if (shader != nullptr) {
				this->geometryShader = shader;
				glUseProgramStages(this->pipelineId, GL_GEOMETRY_SHADER_BIT, std::static_pointer_cast<OGLShader>(shader)->shaderId);
				this->geometryShader->parent = this;
			}
		}

		virtual void setHullShader(const s_ptr<Shader>& shader) override
		{
			if (shader != nullptr) {
				this->hullShader = shader;
				glUseProgramStages(this->pipelineId, GL_TESS_CONTROL_SHADER_BIT, std::static_pointer_cast<OGLShader>(shader)->shaderId);
				this->hullShader->parent = this;
			}
		}

		virtual void setDomainShader(const s_ptr<Shader>& shader) override
		{
			if (shader != nullptr) {
				this->domainShader = shader;
				glUseProgramStages(this->pipelineId, GL_TESS_EVALUATION_SHADER_BIT, std::static_pointer_cast<OGLShader>(shader)->shaderId);
				this->domainShader->parent = this;
			}
		}
	};
}
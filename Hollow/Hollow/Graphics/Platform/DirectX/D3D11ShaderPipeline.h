#pragma once

#include "Hollow/Graphics/ShaderPipeline.h"
#include "D3D11Prerequisites.h"

namespace Hollow {
	class D3D11ShaderPipeline : public ShaderPipeline
	{
	private:
		friend class D3D11ShaderManager;
	public:
		virtual ~D3D11ShaderPipeline() {}
		virtual void setVertexShader(const s_ptr<Shader>& shader) override
		{
			if (shader != nullptr) {
				this->vertexShader = shader;
				this->vertexShader->parent = this;
			}
		}

		virtual void setPixelShader(const s_ptr<Shader>& shader) override
		{
			if (shader != nullptr) {
				this->pixelShader = shader;
				this->pixelShader->parent = this;
			}
		}

		virtual void setGeometryShader(const s_ptr<Shader>& shader) override
		{
			if (shader != nullptr) {
				this->geometryShader = shader;
				this->geometryShader->parent = this;
			}
		}

		virtual void setHullShader(const s_ptr<Shader>& shader) override
		{
			if (shader != nullptr) {
				this->hullShader = shader;
				this->hullShader->parent = this;
			}
		}

		virtual void setDomainShader(const s_ptr<Shader>& shader) override
		{
			if (shader != nullptr) {
				this->domainShader = shader;
				this->domainShader->parent = this;
			}
		}
	};
}
#pragma once

#include "Shader.h"
#include "Hollow/Platform.h"

namespace Hollow {
	struct SHADER_PIPELINE_DESC
	{
		s_ptr<Shader> vertexShader;
		s_ptr<Shader> pixelShader;
		s_ptr<Shader> geometryShader;
		s_ptr<Shader> hullShader;
		s_ptr<Shader> domainShader;
	};

	class ShaderPipeline
	{
	protected:
		friend class ShaderManager;
		s_ptr<Shader> vertexShader;
		s_ptr<Shader> pixelShader;
		s_ptr<Shader> geometryShader;
		s_ptr<Shader> hullShader;
		s_ptr<Shader> domainShader;
	public:
		static s_ptr<ShaderPipeline> create(const SHADER_PIPELINE_DESC& desc);

		virtual void setVertexShader(const s_ptr<Shader>& shader) = 0;
		virtual void setPixelShader(const s_ptr<Shader>& shader) = 0;
		virtual void setGeometryShader(const s_ptr<Shader>& shader) = 0;
		virtual void setHullShader(const s_ptr<Shader>& shader) = 0;
		virtual void setDomainShader(const s_ptr<Shader>& shader) = 0;

		inline s_ptr<Shader>& getVertexShader() { return vertexShader; }
		inline s_ptr<Shader>& getPixelShader() { return pixelShader; }
		inline s_ptr<Shader>& getGeometryShader() { return geometryShader; }
		inline s_ptr<Shader>& getHullShader() { return hullShader; }
		inline s_ptr<Shader>& getDomainShader() { return domainShader; }
	};

	class ComputePipeline
	{
	private:
		friend class ShaderManager;
		s_ptr<Shader> computeShader;
	public:

	};
}
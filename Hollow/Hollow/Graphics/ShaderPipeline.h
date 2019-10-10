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
		s_ptr<Shader>& getVertexShader()
		{
			return vertexShader;
		}

		s_ptr<Shader>& getPixelShader()
		{
			return pixelShader;
		}

		s_ptr<Shader>& getGeometryShader()
		{
			return geometryShader;
		}

		s_ptr<Shader>& getHullShader()
		{
			return hullShader;
		}

		s_ptr<Shader>& getDomainShader()
		{
			return domainShader;
		}
	};

	class ComputePipeline
	{
	private:
		friend class ShaderManager;
		s_ptr<Shader> computeShader;
	public:

	};
}
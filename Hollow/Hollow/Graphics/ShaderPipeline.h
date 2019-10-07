#pragma once

#include "Shader.h"
#include "Hollow/Platform.h"

namespace Hollow {
	class ShaderPipeline
	{
	private:
		friend class ShaderManager;
		s_ptr<Shader> vertexShader;
		s_ptr<Shader> pixelShader;
		s_ptr<Shader> geometryShader;
		s_ptr<Shader> hullShader;
		s_ptr<Shader> domainShader;
	public:

	};

	class ComputePipeline
	{
	private:
		friend class ShaderManager;
		s_ptr<Shader> computeShader;
	public:

	};
}
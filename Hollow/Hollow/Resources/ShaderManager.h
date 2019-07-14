#pragma once
#include "Hollow/Core/CModule.h"
#include "Hollow/Graphics/Base/Shader.h"
#include "Hollow/Graphics/Base/ShaderProgram.h"
#include <string>

namespace Hollow {
	struct SHADER_DESC
	{
		SHADER_TYPE type;
		std::string content;
		SHADER_DESC()
	};

	struct SHADER_PROGRAM_DESC
	{
		Shader* vertex;
		Shader* pixel;
		Shader* geometry;
		Shader* compute;
		Shader* hull;
		Shader* Domain;
	};

	class ShaderManager : public CModule<ShaderManager>
	{
	public:
		virtual Shader* create(SHADER_DESC& desc) = 0;
		virtual ShaderProgram* createShaderProgram(SHADER_PROGRAM_DESC& desc) = 0;
	};
}
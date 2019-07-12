#pragma once
#include "Hollow/Core/CModule.h"
#include "Hollow/Graphics/Renderer/Base/Shader.h"
#include "Hollow/Graphics/Renderer/Base/ShaderProgram.h"
#include <string>

namespace Hollow {
	enum ShaderType
	{
		VERTEX = 0, 
		PIXEL = 1, 
		GEOMERTY = 2, 
		COMPUTE = 3, 
		HULL = 4, 
		DOMAINS = 5
	};

	struct SHADER_DESC
	{
		ShaderType type;
		std::string content;
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
#pragma once

#ifndef HW_SHADER_MANAGER_H
#define HW_SHADER_MANAGER_H

#include "Hollow/Core/CModule.h"
#include "Base/Shader.h"
#include <unordered_map>
#include "Hollow/Platform.h"
#include "Base/ShaderProgram.h"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Common/Helper.h"
#include <string>
#include "Hollow/Common/Logger.h"

namespace Hollow {
	enum SHADER_TYPE
	{
		VERTEX, PIXEL, GEOMERTY, COMPUTE, HULL, DOMAINS
	};

	struct SHADER_DESC
	{
		SHADER_TYPE type;
		std::string content;
		std::string entryPoint;

		SHADER_DESC(SHADER_TYPE type, const std::string& content, const std::string& entryPoint) :
			type(type), content(content), entryPoint(entryPoint)
		{}
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
		virtual Shader* create(const SHADER_DESC& desc) = 0;
		virtual ShaderProgram* createShaderProgram(const SHADER_PROGRAM_DESC& desc) = 0;
	};
}
#endif
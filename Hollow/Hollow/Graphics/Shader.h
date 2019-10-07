#pragma once

#ifndef HW_SHADER_H
#define HW_SHADER_H
#include <string>
#include "CommonTypes.h"

namespace Hollow {
	struct SHADER_DESC
	{
		ShaderType type;
		std::string content;
		std::string entryPoint;

		SHADER_DESC(ShaderType type, const std::string& content, const std::string& entryPoint) :
			type(type), content(content), entryPoint(entryPoint)
		{}
	};

	class Shader
	{
	public:
		ShaderType type;
		std::string shaderContent;
	public:
		static Shader* create(const SHADER_DESC& desc);
		virtual void release() = 0;
	};
}


#endif
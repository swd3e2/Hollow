#pragma once

#ifndef HW_SHADER_H
#define HW_SHADER_H
#include <string>
#include "CommonTypes.h"
#include "Hollow/Platform.h"

namespace Hollow {
	struct SHADER_DESC
	{
		ShaderType type;
		std::string content;
		std::string entryPoint;
		std::string filename;

		SHADER_DESC(ShaderType type, const std::string& filename, const std::string& entryPoint) :
			type(type), filename(filename), entryPoint(entryPoint)
		{}
	};

	class Shader
	{
	public:
		ShaderType type;
		std::string shaderContent;
	public:
		static s_ptr<Shader> create(const SHADER_DESC& desc);
		virtual void release() = 0;
	};
}


#endif
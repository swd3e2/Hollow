#pragma once

#ifndef HW_SHADER_H
#define HW_SHADER_H
#include <string>
#include "CommonTypes.h"
#include "Hollow/Platform.h"

namespace Hollow {
	class ShaderPipeline;

	struct SHADER_DESC
	{
		ShaderType type;
		std::string content;
		std::string entryPoint;
		std::string filename;

		SHADER_DESC() = default;
		SHADER_DESC(ShaderType type, const std::string& filename, const std::string& entryPoint) :
			type(type), filename(filename), entryPoint(entryPoint)
		{}
	};

	class Shader
	{
	public:
		ShaderType type;
		std::string filepath;
		std::string entryPoint;
		ShaderPipeline* parent; // @todo: do we need smart ptr here?
	public:
		static s_ptr<Shader> create(const SHADER_DESC& desc);
		virtual void release() = 0;
	};
}


#endif
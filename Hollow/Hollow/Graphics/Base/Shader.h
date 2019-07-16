#pragma once

#ifndef HW_SHADER_H
#define HW_SHADER_H
#include <string>

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

	class Shader
	{
	public:
		SHADER_TYPE type;
		std::string shaderContent;
	public:
		Shader(SHADER_TYPE type) : type(type) {}
		static Shader* create(const SHADER_DESC& desc);
		virtual void release() = 0;
	};
}


#endif
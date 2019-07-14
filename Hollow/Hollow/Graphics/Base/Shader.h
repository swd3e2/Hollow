#pragma once

#ifndef HW_SHADER_H
#define HW_SHADER_H
#include <string>

namespace Hollow {
	enum SHADER_TYPE
	{
		VERTEX = 0,
		PIXEL = 1,
		GEOMERTY = 2,
		COMPUTE = 3,
		HULL = 4,
		DOMAINS = 5
	};

	class Shader
	{
	public:
		SHADER_TYPE type;
		std::string shaderContent;
	public:
		Shader(SHADER_TYPE type) : type(type) {}

		virtual void release() = 0;
	};
}


#endif
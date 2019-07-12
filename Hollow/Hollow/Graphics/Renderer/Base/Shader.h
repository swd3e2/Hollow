#pragma once

#ifndef HW_SHADER_H
#define HW_SHADER_H
#include <string>

namespace Hollow {
	class Shader
	{
	protected:
	public:
		std::string shaderContent;
	public:
		virtual void release() = 0;
	};
}


#endif
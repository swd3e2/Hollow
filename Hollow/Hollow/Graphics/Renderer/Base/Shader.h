#pragma once

#ifndef HW_SHADER_H
#define HW_SHADER_H

namespace Hollow {
	enum ShaderType
	{
		VERTEX, PIXEL, GEOMERTY, COMPUTE, HULL, DOMAINS
	};

	class Shader
	{
	protected:
		ShaderType type;
	public:
		Shader(ShaderType type) :
			type(type)
		{}

		virtual void release() = 0;
	};
}


#endif
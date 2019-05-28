#pragma once

#include "Hollow/Graphics/Renderer/Base/Shader.h"
#include "OGLPrerequisites.h"
#define DEFAULT_SHADER_ID -1

class OGLShader : public Shader
{
public:
	int shaderId;
	int prevShaderId;
public:
	OGLShader(ShaderType type) :
		Shader(type), shaderId(DEFAULT_SHADER_ID)
	{}

	~OGLShader()
	{
		release();
	}

	virtual void release() override
	{
		if (shaderId != DEFAULT_SHADER_ID) {
			glDeleteShader(shaderId);
		}
	}
};
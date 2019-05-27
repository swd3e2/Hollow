#pragma once

#include "Hollow/Graphics/Renderer/Base/ShaderProgram.h"
#include "OGLPrerequisites.h"

class OGLShaderProgram : public ShaderProgram
{
public:
	int shaderId;
	bool linked = false;
public:
	OGLShaderProgram(Shader* vShader = nullptr, Shader* pShader = nullptr, Shader* gShader = nullptr, Shader* cShader = nullptr)
		: ShaderProgram(vShader, pShader, gShader, cShader)
	{}
	
	OGLShaderProgram(const OGLShaderProgram& other)
	{
		shaderId = other.shaderId;
	}

	OGLShaderProgram(OGLShaderProgram&& other)
	{
		shaderId = other.shaderId;
	}

	OGLShaderProgram& operator=(const OGLShaderProgram& other)
	{
		shaderId = other.shaderId;
		return *this;
	}

	OGLShaderProgram& operator=(OGLShaderProgram&& other)
	{
		shaderId = other.shaderId;
		return *this;
	}

	virtual void release() override
	{
		if (shaderId > 0) {
			glDeleteProgram(shaderId);
		}
	}
};
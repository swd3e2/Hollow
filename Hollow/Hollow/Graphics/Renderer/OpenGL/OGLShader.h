#pragma once

#include "Hollow/Graphics/Renderer/Base/Shader.h"

class OGLShader : public Shader
{
public:
	int shaderId;
public:
	OGLShader(ShaderType type) :
		Shader(type), shaderId(0)
	{}
};
#pragma once

enum ShaderType
{
	VERTEX, PIXEL, GEOMERTY, COMPUTE, HULL, DOMAINS
};

class Shader
{
protected:
	ShaderType type;
protected:
	Shader(ShaderType type) :
		type(type)
	{}
};
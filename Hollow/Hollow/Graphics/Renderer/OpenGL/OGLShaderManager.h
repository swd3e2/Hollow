#pragma once

#include "Hollow/Graphics/ShaderManager.h"
#include "OGLShaderProgram.h"
#include "OGLShader.h"
#include "OGLShaderProgram.h"

class  OGLShaderManager : public ShaderManager
{
public:
	OGLShaderManager();
	virtual Shader* compileShader(ShaderType type, const std::string& path) override;
	virtual ShaderProgram* createShader(Shader* vertexShader, Shader* pixelShader) override;
};
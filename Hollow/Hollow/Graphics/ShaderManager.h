#pragma once

#ifndef HW_SHADER_MANAGER_H
#define HW_SHADER_MANAGER_H

#include "Hollow/Core/CModule.h"
#include "Renderer/Base/Shader.h"
#include <unordered_map>
#include "Hollow/Platform.h"
#include "Renderer/Base/ShaderProgram.h"

class ShaderManager : public CModule<ShaderManager>
{
protected:
	std::unordered_map<std::string, ShaderProgram*> shaders;
public:
	ShaderManager();
	~ShaderManager();

	ShaderProgram* getShader(const std::string& name) { return shaders[name]; }
	virtual Shader* compileShader(ShaderType type, const std::string& path) = 0;
};

#endif
#pragma once

#ifndef HW_SHADER_MANAGER_H
#define HW_SHADER_MANAGER_H

#include "Hollow/Core/CModule.h"
#include "Renderer/Base/Shader.h"
#include <unordered_map>
#include "Hollow/Platform.h"

class ShaderManager : public CModule<ShaderManager>
{
protected:
	std::unordered_map<std::string, Shader*> shaders;
public:
	ShaderManager();
	~ShaderManager();

	Shader* getShader(const std::string& name) { return shaders[name]; }
};

#endif
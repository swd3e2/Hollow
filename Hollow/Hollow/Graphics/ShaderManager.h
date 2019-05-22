#pragma once

#ifndef HW_SHADER_MANAGER_H
#define HW_SHADER_MANAGER_H

#include "Hollow/Core/CModule.h"
#include "Renderer/Base/Shader.h"
#include <unordered_map>
#include "Hollow/Platform.h"
#include "Renderer/Base/ShaderProgram.h"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Common/Helper.h"
#include <string>

class ShaderManager : public CModule<ShaderManager>
{
protected:
	std::unordered_map<std::string, ShaderProgram*> shaders;
	std::string shaderTypeFolder;
	Hollow::FileSystem fs;

	const std::string shaderFolder = "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/";
public:
	ShaderManager();
	~ShaderManager();
	
	inline ShaderProgram* getShader(const std::string& name) { return shaders[name]; }

	virtual ShaderProgram* createShader(Shader* vertexShader, Shader* pixelShader) = 0;
	virtual Shader* compileShader(ShaderType type, const std::string& path) = 0;
};

#endif
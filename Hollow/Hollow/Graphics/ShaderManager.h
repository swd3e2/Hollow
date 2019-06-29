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

namespace Hollow {
	class ShaderManager : public CModule<ShaderManager>
	{
	public:
		std::string shaderTypeFolder;
		std::unordered_map<std::string, ShaderProgram*> shaders;
		std::string shaderFolder;
	protected:
		FileSystem fs;
	public:
		virtual void onShutdown() override;

		virtual void loadShadersFromFolder(const std::string& folder) = 0;
		inline ShaderProgram* getShader(const std::string& name) { return shaders[name]; }

		virtual ShaderProgram* createShader(Shader* vertexShader, Shader* pixelShader, ShaderProgram* prevProgram = nullptr) = 0;
		virtual Shader* compileShader(ShaderType type, const std::string& path, Shader* shader = nullptr) = 0;
		virtual void reloadShader(ShaderProgram* program) = 0;
	};
}

#endif
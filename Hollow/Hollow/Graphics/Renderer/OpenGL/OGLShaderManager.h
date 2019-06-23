#pragma once

#ifndef HW_OGL_SHADER_MANAGER_H
#define HW_OGL_SHADER_MANAGER_H

#include "Hollow/Graphics/ShaderManager.h"
#include "OGLShaderProgram.h"
#include "OGLShader.h"
#include "OGLShaderProgram.h"

namespace Hollow {
	class OGLShaderManager : public ShaderManager
	{
	public:
		OGLShaderManager() = default;

		virtual void loadShadersFromFolder(const std::string& folder) override;
		virtual Shader* compileShader(ShaderType type, const std::string& path, Shader* shader = nullptr) override;
		virtual ShaderProgram* createShader(Shader* vertexShader, Shader* pixelShader, ShaderProgram* prevProgram = nullptr) override;
		virtual void reloadShader(ShaderProgram* program) override;
	};
}

#endif
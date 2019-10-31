#pragma once

#ifndef HW_OGL_SHADER_MANAGER_H
#define HW_OGL_SHADER_MANAGER_H

#include "Hollow/Graphics/ShaderManager.h"
#include "OGLShader.h"
#include "OGLShaderPipeline.h"
#include "Hollow/Common/Log.h"
#include <unordered_map>

namespace Hollow {
	class OGLShaderManager : public ShaderManager
	{
	public:
		virtual s_ptr<Shader> create(const SHADER_DESC& desc) override;
		virtual void reload(const s_ptr<Shader>& shader, std::string shaderContent = "") override;
		virtual s_ptr<ShaderPipeline> create(const SHADER_PIPELINE_DESC& desc) override;
	private:
		bool getError(GLuint shaderId);
		void compileInternal(GLuint& shaderId, const SHADER_DESC& desc);
	};
}

#endif
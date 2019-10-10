#pragma once

#ifndef HW_OGL_SHADER_MANAGER_H
#define HW_OGL_SHADER_MANAGER_H

#include "Hollow/Graphics/ShaderManager.h"
#include "OGLShader.h"
#include "OGLShaderPipeline.h"
#include "Hollow/Common/Log.h"

namespace Hollow {
	class OGLShaderManager : public ShaderManager
	{
	public:
		virtual s_ptr<Shader> create(const SHADER_DESC& desc) override;
		
		virtual s_ptr<ShaderPipeline> create(const SHADER_PIPELINE_DESC& desc) override;
	private:
		void getError(GLuint shaderId)
		{
			GLint isLinked = 0;
			glGetProgramiv(shaderId, GL_LINK_STATUS, &isLinked);

			if (isLinked == GL_FALSE) {
				GLint maxLength = 0;
				glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
				GLchar* infoLog = new GLchar[maxLength];
				glGetProgramInfoLog(shaderId, maxLength, &maxLength, infoLog);
				HW_ERROR("{}", infoLog);
				delete[] infoLog;
			}
		}
	};
}

#endif
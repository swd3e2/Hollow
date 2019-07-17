#pragma once

#ifndef HW_OGL_SHADER_MANAGER_H
#define HW_OGL_SHADER_MANAGER_H

#include "Hollow/Graphics/ShaderManager.h"
#include "OGLShader.h"
#include "Hollow/Common/Log.h"

namespace Hollow {
	class OGLShaderManager : public ShaderManager
	{
	public:
		virtual Shader* create(const SHADER_DESC& desc) override
		{
			const char* shaderCode = desc.content.c_str();

			switch (desc.type)
			{
				case SHADER_TYPE::VERTEX: {
					OGLShader* shader = new OGLShader(SHADER_TYPE::VERTEX);
					shader->shaderId = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &shaderCode);
					getError(shader->shaderId);
					return shader;
				} break;
				case SHADER_TYPE::PIXEL: {
					OGLShader* shader = new OGLShader(SHADER_TYPE::PIXEL);
					shader->shaderId = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &shaderCode);
					getError(shader->shaderId);
					return shader;
				} break;
				default:
					break;
			}
			return nullptr;
		}
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
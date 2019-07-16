#pragma once

#ifndef HW_OGL_SHADER_MANAGER_H
#define HW_OGL_SHADER_MANAGER_H

#include "Hollow/Graphics/ShaderManager.h"
#include "OGLShader.h"

namespace Hollow {
	class OGLShaderManager : public ShaderManager
	{
		virtual Shader* create(const SHADER_DESC& desc) override
		{
			const char* shaderCode = desc.content.c_str();

			switch (desc.type)
			{
				case SHADER_TYPE::VERTEX: {
					OGLShader* shader = new OGLShader(SHADER_TYPE::VERTEX);
					shader->shaderId = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &shaderCode);
					return shader;
				} break;
				case SHADER_TYPE::PIXEL: {
					OGLShader* shader = new OGLShader(SHADER_TYPE::PIXEL);
					shader->shaderId = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &shaderCode);
					return shader;
				} break;
				default:
					break;
			}
			return nullptr;
		}
	};
}

#endif
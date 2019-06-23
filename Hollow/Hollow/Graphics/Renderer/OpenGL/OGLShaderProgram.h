#pragma once

#ifndef HW_OGL_SHADER_PROGRAM_H
#define HW_OGL_SHADER_PROGRAM_H

#include "Hollow/Graphics/Renderer/Base/ShaderProgram.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLShaderProgram : public ShaderProgram
	{
	public:
		int shaderId;
		bool linked = false;
	public:
		OGLShaderProgram(Shader* vShader = nullptr, Shader* pShader = nullptr, Shader* gShader = nullptr, Shader* cShader = nullptr)
			: ShaderProgram(vShader, pShader, gShader, cShader)
		{}

		OGLShaderProgram(const OGLShaderProgram& other) { shaderId = other.shaderId; }
		OGLShaderProgram(OGLShaderProgram&& other) { shaderId = other.shaderId; }

		virtual ~OGLShaderProgram() { 
			release();
			if (pixelShader != nullptr) {
				delete pixelShader;
			}
			if (vertexShader != nullptr) {
				delete vertexShader;
			}
			if (geometryShader != nullptr) {
				delete geometryShader;
			}
			if (computeShader != nullptr) {
				delete computeShader;
			}
			if (hullShader != nullptr) {
				delete hullShader;
			}
			if (domainShader != nullptr) {
				delete domainShader;
			}
		}

		OGLShaderProgram& operator=(const OGLShaderProgram& other)
		{
			shaderId = other.shaderId;
			return *this;
		}

		OGLShaderProgram& operator=(OGLShaderProgram&& other)
		{
			shaderId = other.shaderId;
			return *this;
		}

		virtual void release() override
		{
			if (shaderId > 0) {
				glDeleteProgram(shaderId);
			}
		}
	};
}

#endif
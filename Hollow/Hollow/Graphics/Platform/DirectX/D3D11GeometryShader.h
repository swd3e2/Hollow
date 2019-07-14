#pragma once

#ifndef HW_D3D11_GEOMETRY_SHADER_H
#define HW_D3D11_GEOMETRY_SHADER_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Base/Shader.h"

namespace Hollow {
	class D3D11GeometryShader : public Shader
	{
	private:
		friend class D3D11ShaderManager;
	public:
		D3D11GeometryShader(SHADER_TYPE type) : Shader(type) {}

		~D3D11GeometryShader()
		{
			release();
		}
		virtual void release() override
		{
			SAFE_RELEASE(m_Shader);
		}
		inline ID3D11GeometryShader* GetShader() { return m_Shader; }
	private:
		ID3D11GeometryShader* m_Shader;
	};
}

#endif
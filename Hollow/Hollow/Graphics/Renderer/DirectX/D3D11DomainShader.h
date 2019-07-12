#pragma once

#ifndef HW_D3D11_DOMAIN_SHADER_H
#define HW_D3D11_DOMAIN_SHADER_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Renderer/Base/Shader.h"

namespace Hollow {
	class D3D11DomainShader : public Shader
	{
	private:
		friend class D3D11ShaderManager;
	public:
		virtual void release() override
		{
			SAFE_RELEASE(m_Shader);
		}
		inline ID3D11DomainShader* GetShader() { return m_Shader; }
	private:
		ID3D11DomainShader* m_Shader;
	};
}

#endif
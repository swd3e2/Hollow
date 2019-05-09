#pragma once

#ifndef HW_D3D11_DOMAIN_SHADER_H
#define HW_D3D11_DOMAIN_SHADER_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Renderer/Base/Shader.h"

class D3D11DomainShader : public Shader
{
private:
	friend class D3D11ShaderManager;
public:
	D3D11DomainShader() :
		Shader(ShaderType::DOMAINS)
	{}

	~D3D11DomainShader() {
		SAFE_RELEASE(m_Shader);
		SAFE_RELEASE(m_ShaderBlob);
	}
	inline ID3D11DomainShader* GetShader() { return m_Shader; }
private:
	ID3D11DomainShader* m_Shader;
	ID3DBlob* m_ShaderBlob;
};

#endif
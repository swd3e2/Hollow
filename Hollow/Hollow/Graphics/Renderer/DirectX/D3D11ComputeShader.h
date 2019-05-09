#pragma once

#ifndef HW_D3D11_COMPUTE_SHADER_H
#define HW_D3D11_COMPUTE_SHADER_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Renderer/Base/Shader.h"

class D3D11ComputeShader : public Shader
{
private:
	friend class D3D11ShaderManager;
public:
	D3D11ComputeShader() :
		Shader(ShaderType::COMPUTE)
	{}
	~D3D11ComputeShader() {
		SAFE_RELEASE(m_Shader);
		SAFE_RELEASE(m_ShaderBlob);
	}
	inline ID3D11ComputeShader* GetShader() { return m_Shader; }
private:
	ID3D11ComputeShader* m_Shader;
	ID3DBlob* m_ShaderBlob;
};

#endif
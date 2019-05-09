#pragma once

#ifndef HW_D3D11_VERTEX_SHADER_H
#define HW_D3D11_VERTEX_SHADER_H

#include <wrl/client.h>
#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Renderer/Base/Shader.h"

class D3D11VertexShader : public Shader
{
private:
	friend class D3D11ShaderManager;
public:
	D3D11VertexShader() :
		Shader(ShaderType::VERTEX)
	{}
	~D3D11VertexShader() { 
		SAFE_RELEASE(m_Shader);
		SAFE_RELEASE(m_InputLayout);
		SAFE_RELEASE(m_ShaderBlob);
	}
	inline ID3D11VertexShader* GetShader() { return m_Shader; }
	inline ID3D11InputLayout* GetInputLayout() { return m_InputLayout; }
private:
	ID3D11VertexShader*  m_Shader;
	ID3D11InputLayout* 	 m_InputLayout;
	ID3DBlob* 			 m_ShaderBlob;
};

#endif
#pragma once

#ifndef HW_D3D11_VERTEX_SHADER_H
#define HW_D3D11_VERTEX_SHADER_H

#include "D3D11IShader.h"
#include <wrl/client.h>
#include <string>
#include "D3D11Prerequisites.h"

class D3D11VertexShader : public D3D11IShader
{
public:
	D3D11VertexShader(const std::string filename);

	ID3D11VertexShader * GetShader()
	{
		return m_VertexShader.Get();
	}

	ID3D11InputLayout * GetInputLayout()
	{
		return m_InputLayout.Get();
	}

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>  m_VertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> 	m_InputLayout;
	Microsoft::WRL::ComPtr<ID3DBlob> 			m_VertexShaderBlob;
};

#endif
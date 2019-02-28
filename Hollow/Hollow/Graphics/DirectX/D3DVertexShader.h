#pragma once
#include "D3DIShaderProgramm.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include "Hollow/Platform.h"

class D3DVertexShader : D3DIShaderProgramm
{
public:
	D3DVertexShader(ID3D11Device * device, std::wstring filename)
	{
		HRESULT hr = CompileShader(filename.c_str(), "VSMain", "vs_5_0", m_VertexShaderBlob.GetAddressOf());

		hr = device->CreateVertexShader(m_VertexShaderBlob->GetBufferPointer(),
			m_VertexShaderBlob->GetBufferSize(), NULL, m_VertexShader.GetAddressOf());

		D3D11_INPUT_ELEMENT_DESC bxlayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,							  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		UINT numElements = ARRAYSIZE(bxlayout);

		// Create input layout to vertex shader
		hr = device->CreateInputLayout(bxlayout, numElements,
			m_VertexShaderBlob->GetBufferPointer(), m_VertexShaderBlob->GetBufferSize(),
			m_InputLayout.GetAddressOf());
	}

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
